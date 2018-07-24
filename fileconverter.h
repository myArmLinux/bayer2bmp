#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include "file.h"
#include "errorcodes.h"

#include "converterparams.h"

class Converter {

protected:
    std::string in_f;
    std::string out_f;

public:
    Converter();
    Converter (std::string in_file, std::string out_file);

    virtual bool makeConvert(void) = 0;
};
#endif // FILECONVERTER_H
