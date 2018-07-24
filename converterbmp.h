#ifndef CONVERTERBMP_H
#define CONVERTERBMP_H

#include "converterparams.h"
#include "fileconverter.h"

class ConverterBMP : public Converter
{
private:

    ConverterParams convParams;

    bool generateBmpHeader(std::vector<uint8_t> *bmp_header);

    bool convertRawToBMP(const std::vector<uint16_t> *input_data,
                         std::vector<std::vector<uint8_t> > * out_data);
public:
    ConverterBMP();
    ConverterBMP(const std::string in_file_name, const std::string out_file_name, const ConverterParams *convParams);

    virtual bool makeConvert(void);
};

#endif // CONVERTERBMP_H
