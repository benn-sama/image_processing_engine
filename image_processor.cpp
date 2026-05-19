#include "image_processor.hpp"
/*
 * Returns the location of the ptr where header_size + 1
 */
long Image::ppm_header_size(const std::filesystem::path& fileName) {
    std::ifstream f(fileName, std::ios::binary);
    if (!f) {
        return -1;
    }

    // Skip magic number line (e.g. "P6\n")
    f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int tokens = 0;
    char c{};

    while (tokens < 3) {
        // Skip whitespace
        while (f.get(c) && std::isspace(static_cast<unsigned char>(c)));

        if (!f) return -1;  // malformed file / unexpected EOF

        // Skip comment line
        if (c == '#') {
            f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Skip token (width, height, or maxval)
        while (!std::isspace(static_cast<unsigned char>(c)) && f.get(c));

        ++tokens;
    }

    // tellg points to the first pixel byte
    return static_cast<long>(f.tellg());
}

/*
 * Returns the luminosity function for greyscaling images
 * Formula: (0.2126 * R) + (0.7152 * G) + (0.0722 * B)
 */
int Image::luminosityf(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
}

/*
 * Returns the average method function for greyscaling
 * Formula: (R + G + B) / 3
 */
int Image::avgMethodf(unsigned char const red, unsigned char const green, unsigned char const blue) {
    return (red + green + blue) / 3;
}

/*
 * Returns the lightness function for greyscaling
 * Formula: (max(R, G, B) + min(R, G, B)) / 2
 */
int Image::lightnessf(unsigned char const red, unsigned char const green, unsigned char const blue) {
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

Image::Image() {}

void Image::source(std::string dir) {
    _src = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::binary
    );
}

void Image::dest(std::string dir) {
    _dst = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc
    );
}

void Image::clone() {
    _src->seekg(0, std::ios::end);
    std::streamsize srcSize = _src->tellg();
    _src->seekg(0, std::ios::beg);

    // write from src -> dst
    std::vector<char> cloneBuffer(srcSize);
    _src->read(cloneBuffer.data(), srcSize);
    _dst->write(cloneBuffer.data(), srcSize);
}

void Image::greyscale(char fmethod) {
    std::unique_ptr buffer = std::make_unique<char[]>(3);
    while (_dst->read(buffer.get(), 3)) {
        int red   = (unsigned char)buffer[0];
        int green = (unsigned char)buffer[1];
        int blue  = (unsigned char)buffer[2];

        int grey = 0;

        switch (fmethod) {
            case ('l'):
                grey = this->luminosityf(red, green, blue);
            case ('a'):
                grey = this->avgMethodf(red, green, blue);
            case ('t'):
                grey = this->lightnessf(red, green, blue);
            default:
                grey = this->avgMethodf(red, green, blue);
        }

        _dst->seekp(-3, std::ios::cur); // goes back 3 bytes
        for (int i = 0; i < 3; ++i) {
            _dst->put(grey);
        }
    }
}