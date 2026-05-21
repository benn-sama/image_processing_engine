#ifndef GREYSCALE_HPP
#define GREYSCALE_HPP

#include <vector>

class Greyscale {    
    public:
        Greyscale();
        int luminosityf(unsigned char const red, unsigned char const green, unsigned char const blue);
        int avgMethodf(unsigned char const red, unsigned char const green, unsigned char const blue);
        int lightnessf(unsigned char const red, unsigned char const green, unsigned char const blue);
};

#endif