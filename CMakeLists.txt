cmake_minimum_required(VERSION 3.20)
project(image_processing_engine)

# this tells cmake to use c++20
set(CMAKE_CXX_STANDARD 20)

# this prevents compilation from silently reverting to an old version of c__ 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# this includes the Fetch Module 
include(FetchContent) 

# this calls the git repo before compiling
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0
)

# this calls Catch2 that was declared previously, and adds the dependency in the build tree
FetchContent_MakeAvailable(Catch2)

# adds source files to the build tree, this will be main
add_executable(
    main 
    src/main.cpp 
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

# redirects the tests on where to find proj headers
target_include_directories(main PRIVATE include)

add_executable(
    tests
    src/main.cpp 
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

# where to search for those damn headers
target_include_directories(tests PRIVATE include)
target_link_directories(tests PRIVATE Catch2::Catch2WithMain) # also include the test dependencies