#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <cstddef>
#include <fstream>
#include <memory>
#include <vector>

class Image {
    private:
        std::unique_ptr<std::fstream>src = nullptr;           // source dest. if the image currently being modified
        std::unique_ptr<std::fstream>dst = nullptr;
        std::vector<std::unique_ptr<char>> buffer;  // hold the rgb values
        int offset = 0;                 // size of the src header
        int width  = 0;              // width of the image
        int height = 0;             // width of the image
    public:
        Image();
        void source(std::string dir); // declare src directory
        void dest(std::string dir);   // declare dest directory
        void luminosity();
        void avgMethod();
        void lightness();
        void greyscale();
};

#endif