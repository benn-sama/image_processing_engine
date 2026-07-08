#include "lz77.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include <vector>


LZ77::LZ77() {}

/*
This parses the data

*/
void LZ77::parse() {
    std::string line;

    if (!file.is_open()) {
        std::cout << "File not open." << std::endl;
    }

    // assign var to the content of the buffer/file 
    std::stringstream fileBuffer;
    fileBuffer << file.rdbuf();
    std::string content = fileBuffer.str();

    // split content into unsigned chars
    buffer = std::vector<unsigned char>(content.begin(), content.end());
}
/*
 * Dynamic LZ77 algorithm
 */
void LZ77::compress(std::vector<unsigned char>& data_stream, int const ARR_SIZE) {
    size_t       current_index           = 0;
    size_t       search_window_tail      = ARR_SIZE < MAX_WINDOW_SIZE ? ARR_SIZE : MAX_WINDOW_SIZE;
    size_t       search_window_len       = 8;
    std::unordered_map<std::string, int, string_hash, std::equal_to<>> map;

    // actual lz77 compression methods
    while (current_index < ARR_SIZE) {
        for (; current_index < search_window_tail; ++current_index) {
            std::string_view viewPackage(data_stream[current_index], std::min(search_window_len, buffer.size()));
            
            auto it = map.find(viewPackage);
            if (it == map.end()) {
                map.emplace(std::string(viewPackage), current_index);
                cData.emplace_back(current_index, 0, 0, buffer[current_index + 1]);
            } else {
                cData.emplace_back(Data{(u_int8_t)current_index, u_int8_t(current_index - it->second), 8, buffer[current_index + 1]});
                map.emplace(viewPackage, current_index);
                current_index += 7;
            }
        }

        // quick check if current search window length can support that last numbers
        if (search_window_tail + MAX_WINDOW_SIZE <= ARR_SIZE) {
            search_window_tail += MAX_WINDOW_SIZE;
        } else {
            search_window_tail += (size_t(buffer.size()) - search_window_tail);
            search_window_len  = search_window_tail;
        }

        // restarts the hash map
        map.clear();
    }
}

void LZ77::print() {

    for (auto& data : cData) {
        data.print();
    }
}

void LZ77::printE() {
    for (auto& chara : buffer) {
        std::cout << chara << ' ';
    }
}