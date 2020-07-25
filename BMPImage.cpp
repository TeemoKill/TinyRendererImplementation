#include "BMPImage.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

#define ASSERT(condition,error_message) ((condition)?0:(assertion(__FILE__, __func__, __LINE__,error_message),-1))

inline void assertion(const std::string &filePath, const std::string &function,
                      int line, const std::string &info){
    unsigned long long int pos=filePath.find_last_of('/');
    std::string filename(filePath.substr(pos+1));
    std::string err = filename+" "+function+" "+std::to_string(line)+">>  "+info;
    throw std::runtime_error(err);
}

BMPImage::BMPImage():m_width(0),m_height(0),m_depth(8),m_row_size(0),
                     m_data(nullptr),m_color_palette_size(0),m_color_palette(nullptr) {
    ASSERT(sizeof(int4)==4,"The size of 'long' is not 4 bytes");
    ASSERT(sizeof(uint)==4,"The size of 'unsigned int' is not 4 bytes");
    ASSERT(sizeof(uchar)==1,"The size of 'unsigned char' is not 1 bytes");
    ASSERT(sizeof(uint4)==4,"The size of 'unsigned long' is not 4 bytes");
    ASSERT(sizeof(uint2)==2,"The size of 'unsigned short' is not 2 bytes");
    ASSERT(sizeof(BGR)==3,"The size of 'BGR' is not 3 bytes");
    ASSERT(sizeof(BGRA)==4,"The size of 'BGRA' is not 4 bytes");
    ASSERT(sizeof(ColorPalette)==4,"The size of 'ColorPalette' is not 4 bytes");
}

BMPImage::BMPImage(const char* bmp_file_path):BMPImage() {
    char buffer[500];
    sprintf_s(buffer,"Failed to open bmp image %s\n",bmp_file_path);
    ASSERT(open(bmp_file_path),buffer);
}

BMPImage::BMPImage(int4 width, int4 height, uint2 depth):BMPImage() {
    ASSERT(create(width , height, depth),"Failed to open bmp image\n");
}

BMPImage::BMPImage(const BMPImage &image) {
    this->m_width = image.m_width;
    this->m_height = image.m_height;
    this->m_depth = image.m_depth;
    this->m_row_size = image.m_row_size;

    this->m_file_header = image.m_file_header;
    this->m_info_header = image.m_info_header;

    this->m_color_palette_size = image.m_color_palette_size;
    if(m_color_palette_size > 0){
        this->m_color_palette = new ColorPalette[m_color_palette_size];
        memcpy(m_color_palette,image.m_color_palette,m_color_palette_size * sizeof(ColorPalette));
    } else{
        this->m_color_palette = nullptr;
    }

    if(m_width>0){
        this->m_data = new uchar[m_info_header.biSizeImage];
        memcpy(this->m_data,image.m_data,m_info_header.biSizeImage);
    } else{
        this->m_data = nullptr;
    }

}

BMPImage &BMPImage::operator=(const BMPImage &image) {
    if(this == &image){
        return *this;
    }

    delete[] m_data;
    m_data = nullptr;
    delete[] m_color_palette;
    m_color_palette = nullptr;

    this->m_width = image.m_width;
    this->m_height = image.m_height;
    this->m_depth = image.m_depth;
    this->m_row_size = image.m_row_size;

    this->m_file_header = image.m_file_header;
    this->m_info_header = image.m_info_header;

    this->m_color_palette_size = image.m_color_palette_size;
    if(m_color_palette_size>0){
        this->m_color_palette = new ColorPalette[m_color_palette_size];
        memcpy(m_color_palette,image.m_color_palette,m_color_palette_size * sizeof(ColorPalette));
    } else{
        this->m_color_palette = nullptr;
    }

    if(m_width>0){
        this->m_data = new uchar[m_info_header.biSizeImage];
        memcpy(this->m_data,image.m_data,m_info_header.biSizeImage);
    } else{
        this->m_data = nullptr;
    }

    return *this;
}

BMPImage::~BMPImage() {
    delete[] m_data;
    delete[] m_color_palette;
}

bool BMPImage::open(const char* bmp_file_path) {
    clear();
    ifstream im_file(bmp_file_path,ios::binary);
    if(!im_file.is_open()){
        printf("Failed to open file %s\n",bmp_file_path);
        return false;
    }

    char file_header[14];
    im_file.read(file_header, 14);

    memcpy(&(m_file_header.bfType),file_header,2);
    memcpy(&(m_file_header.bfSize),file_header+2,4);
    memcpy(&(m_file_header.bfReserved1),file_header+6,2);
    memcpy(&(m_file_header.bfReserved2),file_header+8,2);
    memcpy(&(m_file_header.bfOffBits),file_header+10,4);

    if (m_file_header.bfType != 0x4d42) // 0x4d42 = 'BM'
    {
        printf("File %s is not bmp file\n",bmp_file_path);
        return false;
    }

    char info_header[40];
    im_file.read(info_header, 40);

    memcpy(&(m_info_header.biSize),info_header,4);
    memcpy(&(m_info_header.biWidth),info_header+4,4);
    memcpy(&(m_info_header.biHeight),info_header+8,4);
    memcpy(&(m_info_header.biPlanes),info_header+12,2);
    memcpy(&(m_info_header.biBitCount),info_header+14,2);
    memcpy(&(m_info_header.biCompression),info_header+16,4);
    memcpy(&(m_info_header.biSizeImage),info_header+20,4);
    memcpy(&(m_info_header.biXPelsPerMeter),info_header+24,4);
    memcpy(&(m_info_header.biYPelsPerMeter),info_header+28,4);
    memcpy(&(m_info_header.biClrUsed),info_header+32,4);
    memcpy(&(m_info_header.biClrImportant),info_header+36,4);

    if (m_file_header.bfOffBits == 54){
        m_color_palette = nullptr;
    } else{
        m_color_palette_size = m_info_header.biClrUsed;
//        m_color_palette_size = (m_file_header.bfOffBits - 54)/ sizeof(ColorPalette);
        m_color_palette = new ColorPalette[m_color_palette_size];

        im_file.read((char*)(m_color_palette), m_color_palette_size * sizeof(ColorPalette));

    }

    m_width = m_info_header.biWidth;
    m_height = m_info_header.biHeight;
    m_depth = m_info_header.biBitCount;

    m_row_size = 4u * ((m_info_header.biBitCount * m_info_header.biWidth+31u)/32u);

    m_data = new uchar[m_info_header.biSizeImage]();

    im_file.read((char *)m_data,m_info_header.biSizeImage);

    im_file.close();
    return true;
}

bool BMPImage::save(const char* save_path) {
    if (strlen(save_path) == 0){
        printf("Your file path is empty\n");
        return false;
    }
    if (m_width ==0 || m_height == 0){
        printf("This image is empty\n");
        return false;
    }
    ofstream out_image(save_path,ios::binary);
    if (!out_image.is_open()){
        printf("Failed to save image %s\n",save_path);
        return false;
    }

    char file_header[14];

    memcpy(file_header,&(m_file_header.bfType),2);
    memcpy(file_header+2,&(m_file_header.bfSize),4);
    memcpy(file_header+6,&(m_file_header.bfReserved1),2);
    memcpy(file_header+8,&(m_file_header.bfReserved2),2);
    memcpy(file_header+10,&(m_file_header.bfOffBits),4);
    out_image.write(file_header, 14 * sizeof(char));

    char info_header[40];
    memcpy(info_header,&(m_info_header.biSize),4);
    memcpy(info_header+4,&(m_info_header.biWidth),4);
    memcpy(info_header+8,&(m_info_header.biHeight),4);
    memcpy(info_header+12,&(m_info_header.biPlanes),2);
    memcpy(info_header+14,&(m_info_header.biBitCount),2);
    memcpy(info_header+16,&(m_info_header.biCompression),4);
    memcpy(info_header+20,&(m_info_header.biSizeImage),4);
    memcpy(info_header+24,&(m_info_header.biXPelsPerMeter),4);
    memcpy(info_header+28,&(m_info_header.biYPelsPerMeter),4);
    memcpy(info_header+32,&(m_info_header.biClrUsed),4);
    memcpy(info_header+36,&(m_info_header.biClrImportant),4);

    out_image.write(info_header, 40);

    if(m_color_palette_size > 0){
        out_image.write((char*)m_color_palette,m_color_palette_size* sizeof(ColorPalette));
    }

    out_image.write((char*)m_data, m_info_header.biSizeImage);
    out_image.close();
    return true;
}

bool BMPImage::create(int4 width, int4 height, uint2 depth) {
    clear();
    uint4 bytes_per_pixel = 0;
    switch (depth){
        case 8:
            bytes_per_pixel = 1;
            break;
        case 24:
            bytes_per_pixel = 3;
            break;
        case 32:
            bytes_per_pixel = 4;
            break;
        default:
            ASSERT(false,"The depth must be 8, 24 or 32");
    }

    m_width = width;
    m_height = height;
    m_depth = depth;

    m_row_size = 4u * ((bytes_per_pixel * m_width+4u-1u)/4u);

    m_file_header.bfType = 0x4d42;
    if(depth==8) {
        m_file_header.bfOffBits = 14 + 40 + 256* sizeof(ColorPalette);
    } else {
        m_file_header.bfOffBits = 14 + 40;
    }
    m_file_header.bfSize = m_file_header.bfOffBits + m_height * m_row_size;
    m_file_header.bfReserved1 = 0;
    m_file_header.bfReserved2 = 0;

    m_info_header.biSize = 40;//Should be 40
    m_info_header.biWidth = m_width;
    m_info_header.biHeight = m_height;
    m_info_header.biPlanes = 1;
    m_info_header.biBitCount = m_depth;
    m_info_header.biCompression = 0;
    m_info_header.biSizeImage = m_height * m_row_size;
    m_info_header.biXPelsPerMeter = 3780;
    m_info_header.biYPelsPerMeter = 3780;
    m_info_header.biClrUsed = 0;
    m_info_header.biClrImportant = 0;

    if (depth==8){
        m_color_palette_size = 256;
        m_color_palette = new ColorPalette[m_color_palette_size];
        for(uint ii=0;ii<m_color_palette_size;ii++){
            m_color_palette[ii].b = (uchar)ii;
            m_color_palette[ii].g = (uchar)ii;
            m_color_palette[ii].r = (uchar)ii;
            m_color_palette[ii].a = 0;
        }
    } else{
        m_color_palette_size = 0;
        m_color_palette = nullptr;
    }

    m_data = new uchar[m_info_header.biSizeImage]();
    return true;
}

bool BMPImage::valid(int4 x, int4 y, uint2 depth) {
    bool valid = true;
    if(y >= abs(m_height)){
        printf("Index row %d is big than picture height %d-1\n",y,(int)m_height);
        valid = false;
    }
    if(x >= m_width){
        printf("Index col %d is big than picture width %d-1\n", x, (int)m_width);
        valid = false;
    }
    if(m_depth != depth){
        printf("Error: Not a %dbit image\n", depth);
        valid = false;
    }
    if(!valid)
        clear();
    return valid;
}

bool BMPImage::clear() {
    m_width = 0;
    m_height = 0;
    m_depth = 0;
    delete [] m_data;
    m_data = nullptr;
    delete [] m_color_palette;
    m_color_palette = nullptr;
    return true;
}

int4 BMPImage::width() {
    return abs(m_width);
}

int4 BMPImage::height() {
    return abs(m_height);
}

uint2 BMPImage::depth() {
    return m_depth;
}
