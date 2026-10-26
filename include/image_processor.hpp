#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include "greyscale.hpp"
#include "thread.hpp"
#include "thread_pool.hpp"
#include "filter.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <fstream>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <filesystem>
#include <memory>


class Image {
    private:
        std::unique_ptr<std::fstream> _src       = nullptr;  // source dest. if the image currently being modified
        std::unique_ptr<std::fstream> _dst       = nullptr;  // modified file
        std::string                   _srcName   = "";       // dir of source
        std::string                   _dstName   = "";       // dir of dest
        bool                          _modifystr = false;
        bool                          _isValid   = true;
        std::vector<unsigned char>    _currentChunk;
        
        // greyscale related
        Greyscale greyf;
        Filter    gray_filter; //

        // anything image header related
        long  _offset  = 0;  // size of the src header
        int   _width   = 0;  // width of the image
        int   _height  = 0;  // width of the image
        int   _maxval  = 0;  // maxval of the image size of bits in one channel
       
        // these variables keep track of the chunks
        intmax_t _curr_chunk_height = 0; // this tracks which row of chunk it's currently in. SHOULD NEVER GO ABOVE _height
        int      _gcd             = 0;
        int      _chunk_width     = 0;
        int      _chunk_height    = 0;

        // algorithms 
        long ppm_header_size(const std::filesystem::path& fileName);

        // thread pool for work
        ThreadPool thread_pool = ThreadPool(5);
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

        std::vector<std::vector<unsigned char>> get_chunk();
        std::vector<std::vector<unsigned char>> get_chunk_row();
        void extract_finished_chunk();

        // strats
        void strat_A();

        // testing
        void print() {
            std::unique_ptr buffer = std::make_unique<char[]>(3);
            
            while (_src->read(buffer.get(), 1)) {
                std::cout << (int)buffer[0] << std::endl;
            }
        }
        
        // temp for testing
        std::vector<std::vector<unsigned char>> return_img();

        // return gcd
        int get_gcd() { return _gcd; };
};

#endif