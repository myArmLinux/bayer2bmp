#ifndef FILE_H
#define FILE_H

#include "errorcodes.h"
#include <string>

typedef enum {
    IN_FORM_INCORRECT = 0,
    IN_FORM_RGGB = 1,
    IN_FORM_RGBG = 2,
    IN_FORM_GRGB = 3,


    OUT_FORM_BMP = 10,

} input_formats_t;

class myFile {
    private:
        std::string f_name;
        input_formats_t f_format;
        std::pair<int, int> f_size;

        input_formats_t get_input_format(const std::string &in_f_str);
        std::pair<int, int> get_size(const std::string &in_s_str);
        bool check_file_exists(void);

    public:
        static const int params_count = 3;

        myFile(void);
        myFile(const std::string name, const std::string fstr, const std::string size_str);

        std::string get_file_name();
        input_formats_t getF_format() const;
        std::pair<int, int> getF_size() const;

        error_codes check_file_params(void);
};

#endif // FILE_H
