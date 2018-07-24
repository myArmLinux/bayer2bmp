/*
 * This software convert your .raw file with Bayer data to standart .bmp file.
 */

#include <string>
#include <boost/program_options.hpp>

#define LINUX
#undef WINDOWS

#ifdef WINDOWS
    #undef LINUX
    #include <iostream>     // std::cout
    #include <fstream>      // std::ifstream
#endif

#ifdef LINUX
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdint.h>
    #include <unistd.h>
#endif

#include "converterbmp.h"
#include "errorcodes.h"

using namespace boost::program_options;

int main(int argc, char *argv[])
{    
    try {
        options_description desc{"Options"};
        desc.add_options()
          ("help,h", "Help screen")
          ("filename,f", value<std::string>(), "Input raw-file name")
          ("phase,p", value<std::string>()->default_value("GRBG"), "Input data color phase")
          ("size,s", value<std::string>()->default_value("1280x720"), "Input data file size: widthxheight")
          ("bitpp,b", value<int>()->default_value(10), "Input data bit")
          ("offset,o", value<int>()->default_value(0x1400), "Offset to data in input file")
          ("verbose,v", bool_switch()->default_value(false), "Do you want to see debug info?")
          ;

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help")){
            std::cout << desc;
            return 0;
        }

        bool verbose = vm.count("verbose") ? vm["verbose"].as<bool>() : false;
        std::string filename = "";
        std::string outFileName = "";
        std::string color_phase = "";
        std::string widthxheight = "";
        int databit = 0;
        int offset = 0;
        std::string outfilename_prefix = "out_";

        if (vm.count(("filename")))
            filename = vm["filename"].as<std::string>();
        else {
            std::cout << "Enter input filename" << std::endl;
            return 0;
        }

        if (vm.count(("phase")))
            color_phase = vm["phase"].as<std::string>();

        if (vm.count(("size")))
            widthxheight = vm["size"].as<std::string>();

        if (vm.count("bitpp"))
            databit = vm["bitpp"].as<int>();

        if (vm.count("offset"))
            offset = vm["offset"].as<int>();

        if (verbose) {
            std::cout << "Input file name: " << vm["filename"].as<std::string>() << std::endl;
            std::cout << "Color phase: " << vm["phase"].as<std::string>() << std::endl;
            std::cout << "File size: " << vm["size"].as<std::string>() << std::endl;
            std::cout << "File databit: " << vm["bitpp"].as<int>() << std::endl;
        }

        #ifdef WINDOWS
        std::ifstream f(filename.c_str());
        if (!f.good()) {
            std::cout << "No input file!" << std::endl;
            return FILE_NOT_FOUND;
        }
        #else
            struct stat buffer;
            if (!(stat (filename.c_str(), &buffer) == 0))
                std::cout << "No input file!" << std::endl;
        #endif

        ConverterParams params = ConverterParams();
        if (!params.checkAndSetParams(color_phase, widthxheight, databit, offset)) {
            std::cout << "Input parameters incorrect!" << std::endl;
            return INPUT_PARAMS_NOT_RECOGNIZED;
        }

        outFileName = (outfilename_prefix += (filename + ".bmp"));
        ConverterBMP converter(filename, outFileName, &params);
        if (converter.makeConvert())
            std::cout << "Convert finished" << std::endl;

        return 0;
    }
    catch (const error &ex)
    {
      std::cerr << ex.what() << '\n';
    }
}
