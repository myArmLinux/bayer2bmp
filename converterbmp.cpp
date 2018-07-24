#include "converterbmp.h"
#include <math.h>       /* pow */

/*!
 * \brief ConverterBMP::ConverterBMP
 * \param in_file_name
 * \param out_file_name
 * \param convParams
 */
ConverterBMP::ConverterBMP(const std::string in_file_name,
                           const std::string out_file_name,
                           const ConverterParams *convParams) : Converter(in_file_name, out_file_name)
{
    this->convParams = *convParams;
}

/*!
 * \brief ConverterBMP::makeConvert Convert input file to output file.
 * \return
 */
bool ConverterBMP::makeConvert()
{
    /// input data stream
    std::ifstream input;
    input.open( in_f.c_str(), std::ios::in | std::ios::binary );

    /// jutput data stream
    std::ofstream output;
    output.open( out_f.c_str(), std::ios::out | std::ios::binary );

    /// offset to data begin in raw file
    std::streamoff offset = 0x1400;

    /// get length of file:
    input.seekg (0, input.end);
    int length = input.tellg();
    input.seekg (offset, std::ios_base::beg);

    int pixels_count_in_line = convParams.getFile_size().first;
    int line_count = convParams.getFile_size().second;

    std::vector<uint8_t> *bmp_header = new std::vector<uint8_t>();

    if (!generateBmpHeader(bmp_header))
        std::cout << "BMP header not generated" << std::endl;

    std::vector<uint8_t> *id = new std::vector<unsigned char>((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    std::vector<uint16_t> *input_data = (std::vector<uint16_t> *)id;

    std::vector<std::vector<uint8_t> > *out_data = new std::vector<std::vector<uint8_t> >();

    if (!convertRawToBMP(input_data, out_data)) {
        std::cout << "Conversion failed" << std::endl;
    }

    /// write to file
    for (int i = 0; i < bmp_header->size(); i++)
        output << bmp_header->at(i);

    for(std::vector<std::vector<unsigned char> >::reverse_iterator iter = out_data->rbegin(); iter != out_data->rend(); ++iter) {
        for(std::vector<unsigned char>::iterator it = iter->begin(); it != iter->end(); ++it) {
            output << *it;
        }
    }

    output.close();

    return true;
}

/*!
 * \brief ConverterBMP::generateBmpHeader
 * \param bmp_header
 * \return
 */
bool ConverterBMP::generateBmpHeader(std::vector<uint8_t> *bmp_header) {
    int pixels_count_in_line = convParams.getFile_size().first ;
    int line_count = convParams.getFile_size().second;

    ///	The characters "BM"
    bmp_header->push_back(0x42);
    bmp_header->push_back(0x4d);

    /// The size of the file in bytes
    uint32_t size = pixels_count_in_line * line_count * 3 + 14 + 40;
    bmp_header->push_back(size & 0x000000FF);
    bmp_header->push_back((size & 0x0000FF00) >> 8);
    bmp_header->push_back((size & 0x00FF0000) >> 16);
    bmp_header->push_back((size & 0xFF000000) >> 24);

    /// Unused - must be zero
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    /// Unused - must be zero
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    /// Offset to start of Pixel Data
    bmp_header->push_back(0x36);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    /*
       * The Image Header
         * */

    ///biSize BMP file header Size - Must be at least 40
    bmp_header->push_back(0x28);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    ///biWidth Image width in pixels
    uint32_t width = pixels_count_in_line;
    bmp_header->push_back(width & 0x000000FF);
    bmp_header->push_back((width & 0x0000FF00) >> 8);
    bmp_header->push_back((width & 0x00FF0000) >> 16);
    bmp_header->push_back((width & 0xFF000000) >> 24);

    ///biHeight Image height in pixels
    uint32_t height = line_count;
    bmp_header->push_back(height & 0x000000FF);
    bmp_header->push_back((height & 0x0000FF00) >> 8);
    bmp_header->push_back((height & 0x00FF0000) >> 16);
    bmp_header->push_back((height & 0xFF000000) >> 24);

    ///biPlanes Must be 1
    bmp_header->push_back(0x01);
    bmp_header->push_back(0x00);

    ///biBitCount Bits per pixel - 1, 4, 8, 16, 24, or 32
    bmp_header->push_back(24);
    bmp_header->push_back(0x00);

    ///biCompression Compression type (0 = uncompressed)
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    ///biSizeImage Image Size - may be zero for uncompressed images
    uint32_t sz_pic = pixels_count_in_line * line_count * 3;
    bmp_header->push_back(sz_pic & 0x000000FF);
    bmp_header->push_back((sz_pic & 0x0000FF00) >> 8);
    bmp_header->push_back((sz_pic & 0x00FF0000) >> 16);
    bmp_header->push_back((sz_pic & 0xFF000000) >> 24);

    ///biXPelsPerMeter Preferred resolution in pixels per meter by X
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    ///biYPelsPerMeter Preferred resolution in pixels per meter by Y
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    ///biClrUsed Number Color Map entries that are actually used
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    ///biClrImportant Number of significant colors
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);
    bmp_header->push_back(0x00);

    return true;
}

/*!
 * \brief ConverterBMP::convertRawToBMP
 * \param input_data
 * \param out_data
 * \return
 */
bool ConverterBMP::convertRawToBMP(const std::vector<uint16_t> *input_data, std::vector<std::vector<uint8_t> > *out_data) {

    uint16_t max_color_value = pow(2, convParams.getBit_pp()) - 1;
    int pixels_count_in_line = convParams.getFile_size().first;

    std::vector<uint16_t> *prev_line, *next_line;
    uint16_t lt, rt, lb, rb;

    for (int i = 0; i < convParams.getFile_size().second; ++i) {

        std::vector<uint16_t> *line;
        std::vector<uint8_t> *converted_line = new std::vector<uint8_t>();

        // последняя строка в картике или нет
        if (i == (convParams.getFile_size().second - 1)) {
            line = new std::vector<uint16_t>(input_data->begin() + i * pixels_count_in_line,
                                             input_data->begin() + (i + 1)*pixels_count_in_line);
            next_line = prev_line;
        } else {
            line = new std::vector<uint16_t>(input_data->begin() + i * pixels_count_in_line,
                                             input_data->begin() + (i + 1)*pixels_count_in_line);
            next_line = new std::vector<uint16_t>(input_data->begin() + (i + 1)*pixels_count_in_line,
                                                  input_data->begin() + (i + 2)*pixels_count_in_line);
        }

        // посимвольная обработка строки
        for (uint32_t j = 0; j < line->size(); j += 1) {
            if ( j != (line->size() - 1) )
            {
                // не предпоследний пиксель в строке
                lt = line->at(j);
                rt = line->at(j + 1);

                lb = next_line->at(j);
                rb = next_line->at(j + 1);
            }
            else
            {
                // последний пиксель в строке
                lt = line->at(j);
                rt = line->at(j - 1);

                lb = next_line->at(j);
                rb = next_line->at(j - 1);
            }

            uint8_t red8 = 0x00;
            uint8_t blue8 = 0x00;
            uint8_t green8 = 0x00;

            if (((j % 2) == 0) && (i % 2) == 0) {
                if (convParams.getColor_phase() == GRBG) {
                    red8 = rt * 0xFF / max_color_value;
                    blue8 = lb * 0xFF / max_color_value;
                    green8 = lt * 0xFF / max_color_value;
                } else if (convParams.getColor_phase() == RGGB) {

                }
            }
            else if (((j % 2) == 1) && (i % 2) == 0)
            {
                if (convParams.getColor_phase() == GRBG) {
                    red8 = lt * 0xFF / max_color_value;;
                    blue8 = rb * 0xFF / max_color_value;;
                    green8 = rt * 0xFF / max_color_value;
                } else if (convParams.getColor_phase() == RGGB) {

                }
            }
            else if (((j % 2) == 0) && (i % 2) == 1)
            {
                if (convParams.getColor_phase() == GRBG) {
                    red8 = rb * 0xFF / max_color_value;
                    blue8 = lt * 0xFF / max_color_value;
                    green8 = rt * 0xFF / max_color_value;
                } else if (convParams.getColor_phase() == RGGB) {

                }
            }
            else if (((j % 2) == 1) && (i % 2) == 1)
            {
                if (convParams.getColor_phase() == GRBG) {
                    red8 = lb * 0xFF / max_color_value;
                    blue8 = rt * 0xFF / max_color_value;
                    green8 = lt * 0xFF / max_color_value;
                } else if (convParams.getColor_phase() == RGGB) {

                }
            }

            // red green blue
            converted_line->push_back(blue8);
            converted_line->push_back(green8);
            converted_line->push_back(red8);
        }

        out_data->push_back(*converted_line);
        prev_line = line;
    }
    return true;
}
