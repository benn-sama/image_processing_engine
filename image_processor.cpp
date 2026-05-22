#include "image_processor.hpp"
#include <filesystem>
#include <memory>
#include <ratio>
/*
 * Returns the location of the ptr where header_size + 1
 */
long Image::ppm_header_size(const std::filesystem::path& fileName) {
    std::ifstream f(fileName, std::ios::binary);
    if (!f) { return -1; }

    // Skip magic number line (e.g. "P6\n")
    f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int tokens = 0;
    char c{};
    int* targets[] = { &_width, &_height, &_maxval };

    while (tokens < 3) {
        // Skip whitespace
        while (f.get(c) && std::isspace(static_cast<unsigned char>(c)));

        if (!f) { return -1; }

        // Skip comment line
        if (c == '#') {
            f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Put back the first digit and read one token
        f.putback(c);
        f >> *targets[tokens];

        if (!f) { return -1; }

        ++tokens;
    }

    // Consume the single required whitespace after maxval
    f.ignore(1);

    // tellg now points to the first pixel byte
    return static_cast<long>(f.tellg());
}

Image::Image() {}

void Image::source(std::string &dir) {
    _src = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::binary
    );

    _srcName = dir;
    _offset = this->ppm_header_size(dir); // this also initializes width, height, and maxval

    if (_offset <= 0) { _isValid = false; }
}

void Image::dest(std::string &dir) {
    _dst = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc
    );

    _dstName = dir;
}

void Image::clone() {
    _src->seekg(0, std::ios::end);
    std::streamsize srcSize = _src->tellg();
    _src->seekg(0, std::ios::beg);

    // write from src -> dst
    std::vector<char> cloneBuffer(srcSize);
    _src->read(cloneBuffer.data(), srcSize);
    _dst->write(cloneBuffer.data(), srcSize);
    
    _dst->seekg(_offset, std::ios::beg);
}

void Image::greyscale(char &fmethod) {
    if (_src == nullptr) {
        std::cout << "No src found\n";
        return;
    }

    if (_dst == nullptr) {
        std::cout << "No dst found\n";

        std::string tempDir     = _srcName;
        int         dotppmIndex = tempDir.find(".ppm"); 
        std::string newDir      = tempDir.insert(dotppmIndex, " copy");
        int         appendIndex = 0;

        namespace fs = std::filesystem;

        // this keeps appending infinitely until finds one that isn't taken
        while (fs::exists(newDir)) {
            appendIndex = appendIndex + 5;
            newDir = tempDir.insert(dotppmIndex + appendIndex, " copy");
        }

        _dst = std::make_unique<std::fstream>(
            newDir,
            std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc
        );
    }

    std::cout << "cloning file.\n";
    this->clone();
    std::cout << "cloned file.\n";

    std::unique_ptr buffer = std::make_unique<char[]>(3);
    while (_dst->read(buffer.get(), 3)) {
        int red   = (unsigned char)buffer[0];
        int green = (unsigned char)buffer[1];
        int blue  = (unsigned char)buffer[2];

        int grey = 0;

        switch (fmethod) {
            case ('l'):
                grey = greyf.luminosityf(red, green, blue);
                break;
            case ('a'):
                grey = greyf.avgMethodf(red, green, blue);
                break;
            case ('t'):
                grey = greyf.lightnessf(red, green, blue);
                break;
            default:
                grey = greyf.avgMethodf(red, green, blue);
                break;
        }

        _dst->seekp(-3, std::ios::cur); // goes back 3 bytes
        for (int i = 0; i < 3; ++i) {
            _dst->put(grey);
        }

        // ++count;
        // std::cout << "count: " << count << std::endl;
    }
}