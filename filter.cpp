#include "filter.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <sys/types.h>
#include <cmath>
#include <vector>

Filter::Filter(size_t const channel, size_t const bit_depth) {
    
}

// this can be written better but i'm lazy lol
int Filter::subf(int const current_byte, int const go_back_n) {
    int diff = current_byte - go_back_n;

    if (diff < 0) {
        return diff + 256;
    } else {
        return diff % 256;
    }
}

/*
sub(x) = [curr_byte - bpp] mod 256
bpp    = curr_byte then go back n (if x - 3 < 0, then it is 0)
n      = (channel * bit_depth) / 8
*/
void Filter::sub(std::vector<unsigned char>& bottom, std::vector<unsigned char>& subv, int const ARR_SIZE) {
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
void Filter::up(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& subv, int const ARR_SIZE) {
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
    for (int i = 0; i < ARR_SIZE; ++i) {
       int prev = (i < _bpp) ? 0 : top[i];

       avgv[i] = avgf(bottom[i], top[i], prev);
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

    if (current_distance <= left_distance && current_distance <= top_left_distance) {
        return up;
    } else if (left_distance <= top_left_distance) {
        return left;
    } else {
        return top_left;
    }
}

void Filter::paeth(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& paethv, int const ARR_SIZE) {
    for (int i = 0; i < ARR_SIZE; ++i) {
        int left     = i < _bpp ? 0 : (int)bottom[i];
        int up       = (int)top[i];
        int top_left = i < 0 ? 0 : (int)top[i - 1];
        
        paethv[i] = bottom[i] - paethf(up, left, top_left); 
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
    sub(bottom, subv, ARR_SIZE);
    up(bottom, top, upv, ARR_SIZE);
    avg(bottom, top, avgv, ARR_SIZE);
    paeth(bottom, top, paethv, ARR_SIZE);
}