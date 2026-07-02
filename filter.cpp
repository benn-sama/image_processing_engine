#include "filter.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <sys/types.h>
#include <cmath>
#include <vector>
#include <limits>

Filter::Filter(size_t const channel, size_t const bit_depth) {
   _bpp = (channel * bit_depth) / 8; 
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
int Filter::sub(std::vector<unsigned char>& bottom, std::vector<std::vector<unsigned char>>& filtersv, int const ARR_SIZE) {
    int sum = 0;
    int sub = 0;
    
    for (size_t i = 1; i < ARR_SIZE; ++i) {
        int prev = (i < _bpp) ? 0 : bottom[i - _bpp];
        sub      = subf((int)bottom[i], prev);
        filtersv[1][i]  = sub;
        sum     += sub;
    }

    return sum / ARR_SIZE;
}

int Filter::upf(int const current_byte, int const prior) {
    return current_byte < prior ? current_byte - prior + 256: current_byte - prior;
}

/*
up() = raw(x) - prior(x)
prior(x) = byte of previous line of index j, [i - 1][j]
*/
int Filter::up(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<std::vector<unsigned char>>& filtersv, int const ARR_SIZE) {
    int sum = 0;
    int up  = 0;
    
    for (int i = 1; i < ARR_SIZE; ++i) {
        up     = upf((int)bottom[i], top[i]); 
        filtersv[2][i] = up;
        sum   += up;
    }

    return sum / ARR_SIZE;
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
int Filter::avg(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<std::vector<unsigned char>>& filtersv, int const ARR_SIZE) {
    int sum = 0;
    int avg = 0;

    for (int i = 1; i < ARR_SIZE; ++i) {
       int prev = (i < _bpp) ? 0 : top[i];

       avg     = avgf(bottom[i], top[i], prev);
       filtersv[3][i] = avg;
       sum    += avg;
    }

    return sum / ARR_SIZE;
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

int Filter::paeth(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<std::vector<unsigned char>>& filtersv, int const ARR_SIZE) {
    int sum   = 0;
    int paeth = 0;

    for (int i = 1; i < ARR_SIZE; ++i) {
        int left     = i < _bpp ? 0 : (int)bottom[i];
        int up       = (int)top[i];
        int top_left = i < 0 ? 0 : (int)top[i - 1];
        
        paeth     = bottom[i] - paethf(up, left, top_left); 
        filtersv[4][i] = paeth; 
        sum      += paeth;
    }
    
    return sum / ARR_SIZE;
}


int Filter::none(std::vector<unsigned char>& bottom, std::vector<std::vector<unsigned char>>& filtersv, int const ARR_SIZE) {
    int sum = 0;

    for (int i = 1; i < ARR_SIZE; ++i) {
        filtersv[4][i] = bottom[i];
        sum += filtersv[0][i];
    }

    return sum / ARR_SIZE;
}
/*
ideas:
pass in 2 std::array<unsigned char> references, one top, other bottom. Then return the array
pass in the full scanline as a reference, but we have to track other rigerminrole stuff
*/

void Filter::filter_scanline(std::vector<unsigned char>& top, std::vector<unsigned char>& bottom, std::vector<unsigned char>& alter, int const ARR_SIZE) {
    // preallocate 
    int VEC_SIZE = ARR_SIZE + 1;
    std::array<int, 5>         dumb_arr;

    std::vector<std::vector<unsigned char>> filtersv(VEC_SIZE, std::vector<unsigned char>(5));

    filtersv[0].push_back(0);
    filtersv[1].push_back(1);
    filtersv[2].push_back(2);
    filtersv[3].push_back(3);
    filtersv[4].push_back(4);

    /*
    1. subf
    2. upf
    3. avgf
    4. paethf
    */
    dumb_arr[0] = none(bottom, filtersv, ARR_SIZE);
    dumb_arr[1] = sub(bottom, filtersv, ARR_SIZE);
    dumb_arr[2] = up(bottom, top, filtersv, ARR_SIZE);
    dumb_arr[3] = avg(bottom, top, filtersv, ARR_SIZE);
    dumb_arr[4] = paeth(bottom, top, filtersv, ARR_SIZE);

    int max = dumb_arr[0];

    for (int i = 1; i < 5; ++i) {
        if (max < dumb_arr[i]) {
            max = dumb_arr[i];
        }
    }

    for (int i = 0; i < VEC_SIZE; ++i) {
        alter[i] = filtersv[max][i];
    }
}