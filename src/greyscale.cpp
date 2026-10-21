#include "greyscale.hpp"

/*
 * Returns the luminosity function for greyscaling Greyscales
 * Formula: (0.2126 * R) + (0.7152 * G) + (0.0722 * B)
 */
int Greyscale::luminosityf(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
}

/*
 * Returns the average method function for greyscaling
 * Formula: (R + G + B) / 3
 */
int Greyscale::avgMethodf(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (red + green + blue) / 3;
}

/*
 * Returns the lightness function for greyscaling
 * Formula: (max(R, G, B) + min(R, G, B)) / 2
 */
int Greyscale::lightnessf(unsigned char const red, unsigned char const green, unsigned char const blue) {
    std::vector<unsigned char> rgb{red, green, blue};
    unsigned char min = red;
    unsigned char max = red;

    for (auto& color : rgb) {
        if (color < min) {
            min = color;
        }

        if (color > max) {
            max = color;
        }
    }

    
    return (max + min) / 2;
}