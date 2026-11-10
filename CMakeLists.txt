cmake_minimum_required(VERSION 3.20)
project(image_processing_engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # fail instead of silently falling back to an older standard
set(CMAKE_CXX_EXTENSIONS OFF)         # -std=c++20 

# Writes compile_commands.json to the build dir so clangd / clang-tidy / IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Sanitizers -------------------------------------------------------------
# Defined BEFORE FetchContent so dependencies get instrumented as well
option(ENABLE_TSAN "Build with ThreadSanitizer" OFF)
if(ENABLE_TSAN)
  add_compile_options(-fsanitize=thread -g -O1 -fno-omit-frame-pointer)
  add_link_options(-fsanitize=thread)
endif()

# ---- Dependencies -----------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.11.0
)
FetchContent_MakeAvailable(Catch2)

find_package(Threads REQUIRED)

# ---- Engine library: compiled once, shared by the app and the tests --------
add_library(engine STATIC
  src/filter.cpp
  src/greyscale.cpp
  src/huffman.cpp
  src/image_processor.cpp
  src/lz77.cpp
  src/pngt.cpp
  src/random_num_gen.cpp
  src/thread_pool.cpp
  src/thread.cpp
)
# PUBLIC = anything linking `engine` automatically gets these headers and pthreads
target_include_directories(engine PUBLIC include)
target_link_libraries(engine PUBLIC Threads::Threads)

# ---- Application ------------------------------------------------------------
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE engine)

# ---- Tests ------------------------------------------------------------------
# No src/main.cpp here: Catch2WithMain provides main().
add_executable(tests
  tests/image_processor_test.cpp
  tests/thread_pool_test.cpp
)
target_link_libraries(tests PRIVATE engine Catch2::Catch2WithMain)

enable_testing()
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)   # needed for include(Catch) with FetchContent
include(Catch)
catch_discover_tests(tests
  PROPERTIES
    TIMEOUT 60   # kill hung tests (e.g. deadlocked thread pool)
    ENVIRONMENT "TSAN_OPTIONS=halt_on_error=1:second_deadlock_stack=1"
)