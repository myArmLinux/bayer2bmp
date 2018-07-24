#ifndef CONVERTERPARAMS_H
#define CONVERTERPARAMS_H

#include <bits/stl_pair.h>
#include <string>

typedef enum {
    GRBG = 0,
    RGGB,
    BGGR,
} color_formats;

class ConverterParams
{
private:
    color_formats color_phase;
    std::pair<int, int> file_size;
    int bit_pp;
    int offset;

    bool parseColorPhase(const std::string color_fmt);
    bool parseFileSize(const std::string size);
    bool parseBPP(const int bpp);
    bool parseOffset(const int offset);

public:
    ConverterParams();
    bool checkAndSetParams(const std::string c_phase, const std::string size, const int databit, const int offset);

    color_formats getColor_phase() const;
    void setColor_phase(const color_formats &value);
    std::pair<int, int> getFile_size() const;
    void setFile_size(const std::pair<int, int> &value);
    int getBit_pp() const;
    void setBit_pp(int value);
    int getOffset() const;
    void setOffset(int value);
};

#endif // CONVERTERPARAMS_H
