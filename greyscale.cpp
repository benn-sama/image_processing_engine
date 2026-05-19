#include <vector>

/*
 * Returns the luminosity function for greyscaling images
 * Formula: (0.2126 * R) + (0.7152 * G) + (0.0722 * B)
 */
inline unsigned char luminosity(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
}

/*
 * Returns the average method function for greyscaling
 * Formula: (R + G + B) / 3
 */
inline unsigned char avgMethod(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (red + green + blue) / 3;
}

/*
 * Returns the lightness function for greyscaling
 * Formula: (max(R, G, B) + min(R, G, B)) / 2
 */
inline unsigned char lightness(unsigned char const red, unsigned char const green, unsigned char const blue) {
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

