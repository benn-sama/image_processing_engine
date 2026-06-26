#include "filter.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <sys/types.h>
#include <cmath>
#include <vector>

// this shouldn't assume that it is offset, must fix later
void Filter::allocate(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream) {
    std::unique_ptr buffer = std::make_unique<char[]>(1);
    _width  = width;
    _height = height;
    
    // w = 2268, h = 4032
    std::cout << "width: " << width << std::endl << "height: " << height << std::endl;

    // stores the the old img, just in case for later
    // allocates new img, so we don't have to allocate memory, since it will always be allocated already
    for (size_t i = 0; i < width; ++i) {
        _old_img.push_back({}); // create a row before accessing
        _new_img.push_back({});
        for (size_t j = 0; j < height; ++j) {
            buffer_stream->read(buffer.get(), 1);
            _old_img[i].push_back(buffer[0]);
            _new_img[i].push_back(buffer[0]);
            // std::cout << "pushed: " << "[" << i << "]" << "[" << j << "]" << " = " << (int)_old_img[i][j] << std::endl;
        }
    }
    
    // points ptr to the offset 
    buffer_stream->seekg(offset, std::ios::beg);
}

void Filter::verify(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream) {
    std::unique_ptr buffer = std::make_unique<char[]>(1);

    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            buffer_stream->read(buffer.get(), 1);
            if (_old_img[i][j] != (u_int8_t)buffer[0]) {
                // std::cout << "[" << i << "]" << "[" << j << "] " << " = " << (int)_old_img[i][j] << " != " << (u_int8_t)buffer[0] << std::endl;
                exit(1);
            }
        }
    }

    buffer_stream->seekg(offset, std::ios::beg);
}

int Filter::subf(int const current_byte, int const go_back_n) {
    return go_back_n <= 0 ? 0 : (current_byte - go_back_n) % 256;
}

/*
sub(x) = [curr_byte - bpp] mod 256
bpp    = curr_byte then go back n (if x - 3 < 0, then it is 0)
n      = (channel * bit_depth) / 8
*/
void Filter::sub(std::vector<unsigned char>& bottom, std::vector<unsigned char>& subv, int const ARR_SIZE) {
    // for (size_t i = 0; i < _width; ++i) {
    //     for (size_t j = 0; j < _height; ++j) {
    //         int go_back_n = (j < _bpp) ? 0 : _old_img[i][j - _bpp];
    //         _new_img[i][j] = subf((int)_old_img[i][j], go_back_n);
    //     }
    // }

    for (size_t i = 0; i < ARR_SIZE; ++i) {
        int prev = (i < _bpp) ? 0 : bottom[i - _bpp];
        subv[i] = subf((int)bottom[i], prev);
    }
}

int Filter::upf(int const current_byte, int const prior) {
    return current_byte < prior ? current_byte - prior + 256: current_byte - prior;
}

/*
up() = raw(x) - prior(x)
prior(x) = byte of previous line of index j, [i - 1][j]
*/
void Filter::up(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& upv, int const ARR_SIZE) {
    // for (int i = 0; i < _width; ++i) {
    //     for (int j = 0; j < _height; ++j) {
    //         int prior = i <= 0 ? 0 : (int)_old_img[i - 1][j];
    //         _new_img[i][j] = upf((int)_old_img[i][j], prior);
    //     }
    // }

    for (int i = 0; i < ARR_SIZE; ++i) {
        subv[i] = upf((int)bottom[i], top[i]);
    }
}

/*
avg(x) = raw(x) - floor((raw(x - bpp) + prior(x)) / 2)
floor() = round to the smallest whole int
bpp = see header
prior(x) = previous scanline byte = [i - 1][j] 
*/
int Filter::avgf(int const current_byte, int const prior, int const go_back_n) {
    int diff = (current_byte - (int)std::floor((go_back_n + prior) / 2)); 
    return  diff < 0 ? diff + 256 : diff;
}

// this is identical to up's algorithm (prolly can do something about it)
void Filter::avg(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& avgv, int const ARR_SIZE) {
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            int prior = i <= 0 ? 0 : (int)_old_img[i - 1][j];
            int go_back_n = (j < _bpp) ? 0 : _old_img[i][j - _bpp];
            
            _new_img[i][j] = avgf((int)_old_img[i][j], prior, go_back_n);
        }
    }

    for (int i = 0; i < ARR_SIZE; ++i) {
       int prev = (i < _bpp) ? 0 : top[i];

       avg
    }
}

/*
paeth(x) = raw(x) - paethPredictor(raw(x - bpp), prior(x), prior(x-bpp))
a = raw(x - bpp) // left of current byte
b = prior(x)     
c = prior(x-bpp)
*/
int Filter::paethf(int const up, int const left, int const top_left) {
    int init_est          = up + left - top_left;
    int current_distance  = std::abs(init_est - up);
    int left_distance     = std::abs(init_est - left);
    int top_left_distance = std::abs(init_est - top_left);

    if (current_distance <= left_distance && up <= top_left_distance) {
        return up;
    } else if (left_distance <= top_left_distance) {
        return left;
    } else {
        return top_left;
    }
}

void Filter::paeth() {
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            int left     = j < _bpp ? 0 : _old_img[i][j - _bpp];          // check if left is valid
            int up       = i < 1 ? 0 : _old_img[i - 1][j];
            int top_left = (i < 1 || j < 1) ? 0 : _old_img[i - 1][j - 1]; // check if top left is valid

            _new_img[i][j] = _old_img[i][j] - paethf(up, left, top_left);
        }
    }
}

/*
ideas:
pass in 2 std::array<unsigned char> references, one top, other bottom. Then return the array
pass in the full scanline as a reference, but we have to track other rigerminrole stuff
*/
void Filter::filter_scanline(std::vector<unsigned char>& top, std::vector<unsigned char>& bottom, std::vector<unsigned char>& alter, int const ARR_SIZE) {
    // preallocate 
    std::vector<unsigned char> subv(ARR_SIZE);
    std::vector<unsigned char> upv(ARR_SIZE);
    std::vector<unsigned char> avgv(ARR_SIZE);
    std::vector<unsigned char> paethv(ARR_SIZE);

    /*
    1. subf
    2. upf
    3. avgf
    4. paethf
    */


}