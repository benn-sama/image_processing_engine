#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <fstream>
#include <memory>
#include <vector>
#include <fstream>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <ios>
#include <iostream>

class Image {
    private:
        std::unique_ptr<std::fstream> _src       = nullptr;  // source dest. if the image currently being modified
        std::unique_ptr<std::fstream> _dst       = nullptr;  // modified file
        std::string                   _srcName   = "";       // dir of source
        std::string                   _dstName   = "";       // dir of dest
        bool                          _modifystr = false;
        bool                          _isValid   = true;

        // anything image header related
        long                          _offset  = 0;  // size of the src header
        int                           _width   = 0;  // width of the image
        int                           _height  = 0;  // width of the image
        int                           _maxval  = 0;  // maxval of the image

        // algorithms 
        long ppm_header_size(const std::filesystem::path& fileName);
        int luminosityf(unsigned char const red, unsigned char const green, unsigned char const blue);
        int avgMethodf(unsigned char const red, unsigned char const green, unsigned char const blue);
        int lightnessf(unsigned char const red, unsigned char const green, unsigned char const blue);


    public:
        Image();
        void source(std::string &dir); // declare src directory
        void dest(std::string &dir);   // declare dest directory
        void clone();
        void greyscale(char &fmethod);
};

#endif