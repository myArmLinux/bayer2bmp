#include "converterparams.h"
#include <sstream>
#include <iterator>
#include <vector>

color_formats ConverterParams::getColor_phase() const
{
    return color_phase;
}

void ConverterParams::setColor_phase(const color_formats &value)
{
    color_phase = value;
}

std::pair<int, int> ConverterParams::getFile_size() const
{
    return file_size;
}

void ConverterParams::setFile_size(const std::pair<int, int> &value)
{
    file_size = value;
}

int ConverterParams::getBit_pp() const
{
    return bit_pp;
}

void ConverterParams::setBit_pp(int value)
{
    bit_pp = value;
}

int ConverterParams::getOffset() const
{
    return offset;
}

void ConverterParams::setOffset(int value)
{
    offset = value;
}

bool ConverterParams::parseColorPhase(const std::string color_fmt) {
    if (color_fmt.compare("RGGB") == 0) {
        this->color_phase = RGGB;
        return true;
    }
    else if (color_fmt.compare("GRBG") == 0) {
        this->color_phase = GRBG;
        return true;
    }
    else if (color_fmt.compare("BGGR") == 0) {
        this->color_phase = BGGR;
        return true;
    }
    else
        return false;
}

bool ConverterParams::parseFileSize(const std::string size)
{
    std::istringstream iss(size);
    std::vector<std::string> results;
    std::string token;

    while(std::getline(iss, token, 'x')) {
        results.push_back(token);
    }

    if (results.size() == 2){
        this->file_size.first = std::stoi(results.at(0));
        this->file_size.second = std::stoi(results.at(1));
        return true;
    }

    return false;
}

bool ConverterParams::parseBPP(const int bpp)
{
    if (bpp != 10 && bpp != 16)
        return false;
    else
        setBit_pp(bpp);

    return true;
}

bool ConverterParams::parseOffset(const int offset)
{
    if (offset < 0)
        return false;
    else
        setOffset(offset);

    return true;
}

ConverterParams::ConverterParams()
{

}

/*!
 * \brief ConverterParams::checkAndSetParams Check input params and init fields of class.
 * \param c_phase
 * \param size
 * \param databit
 * \param offset
 * \return
 */
bool ConverterParams::checkAndSetParams(const std::string c_phase, const std::string size, const int databit, const int offset)
{
    if (!parseColorPhase(c_phase))
        return false;

    if (!parseFileSize(size))
        return false;

    if (!parseBPP(databit))
        return false;

    if (!parseOffset(offset))
        return false;

    return true;
}
