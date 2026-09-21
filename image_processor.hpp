#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include "greyscale.hpp"
#include "/thread_pool/thread_pool.hpp"
#include "filter.hpp"

#include <fstream>
#include <memory>
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
        
        // greyscale related
        Greyscale greyf;
        Filter    gray_filter; //

        // anything image header related
        long                          _offset  = 0;  // size of the src header
        int                           _width   = 0;  // width of the image
        int                           _height  = 0;  // width of the image
        int                           _maxval  = 0;  // maxval of the image size of bits in one channel

        // algorithms 
        long ppm_header_size(const std::filesystem::path& fileName);

        // thread pool for work
        ThreadPool thread_pool;
    public:
        Image(std::string dir);
        void source(std::string &dir); // declare src directory
        void dest(std::string &dir);   // declare dest directory
        void clone();
        /*
        l = luminosity
        a = avg
        t = lightness
        */
        void greyscale(char &fmethod);
        void greyscale_adaptive();
        std::fstream* get_ptr() { return _src.get(); };
        int get_width()   { return _width;  }
        int get_height()  { return _height; }
        long get_offset() { return _offset; };

        // testing
        void print() {
            std::unique_ptr buffer = std::make_unique<char[]>(3);
            
            while (_src->read(buffer.get(), 1)) {
                std::cout << (int)buffer[0] << std::endl;
            }
        }
};

#endif