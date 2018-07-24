#include "file.h"
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#define LINUX

#ifdef LINUX
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdint.h>
    #include <unistd.h>
#endif


std::pair<int, int> myFile::getF_size() const
{
return f_size;
}

input_formats_t myFile::get_input_format(const std::string &in_f_str) {
    if (in_f_str == "RGGB")
        return IN_FORM_RGGB;
    else if (in_f_str == "bmp")
        return OUT_FORM_BMP;
    else
        return IN_FORM_INCORRECT;
}

std::pair<int, int> myFile::get_size(const std::string &in_s_str) {
    return std::pair<int, int>(1280, 720);
}

bool myFile::check_file_exists()
{
#ifdef WINDOWS
    std::ifstream f(f_name.c_str());
    return f.good();
#else
    struct stat buffer;
    return (stat (f_name.c_str(), &buffer) == 0);
#endif
}

myFile::myFile()
{

}

myFile::myFile(const std::string name, const std::string fstr, const std::string size_str) {
    f_name = name;
    f_format = get_input_format(fstr);
    f_size = get_size(size_str);
}

std::string myFile::get_file_name() {
    return f_name;
}

error_codes myFile::check_file_params() {
    if (f_size.first == 0 || f_size.second == 0)
        return INPUT_SIZE_ERROR;

    if (f_format == 0)
        return INPUT_FORMAT_NOT_RECOGNIZED;

    if (!check_file_exists())
        return FILE_NOT_FOUND;

    return ALL_RIGHT;
}

input_formats_t myFile::getF_format() const
{
    return f_format;
}
