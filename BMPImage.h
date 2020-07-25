#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <string>
#include <stdexcept>

#ifndef NDEBUG
#include <typeinfo>
#include <cstring>

#endif

typedef int int4;              // 4 bytes
typedef unsigned int uint;     // 4 bytes
typedef unsigned char uchar;   // 1 byte
typedef unsigned int uint4;   // 4 bytes
typedef unsigned short uint2;  // 2 bytes


typedef struct {
    uchar b;
    uchar g;
    uchar r;
    uchar a;
}BGRA,ColorPalette;

typedef struct {
    uchar b;
    uchar g;
    uchar r;
}BGR;

class BMPImage {
private:

    typedef struct {
        uint2 bfType;
        uint4 bfSize;
        uint2 bfReserved1;
        uint2 bfReserved2;
        uint4 bfOffBits;
    } BMPFileHeader;

    typedef struct {
        uint4 biSize;
        int4 biWidth;
        int4 biHeight;
        uint2 biPlanes;
        uint2 biBitCount;
        uint4 biCompression;
        uint4 biSizeImage;
        int4 biXPelsPerMeter;
        int4 biYPelsPerMeter;
        uint4 biClrUsed;
        uint4 biClrImportant;
    } BMPInfoHeader;


public:

    explicit BMPImage();

    explicit BMPImage(const char* bmp_file_path);

    explicit BMPImage(int4 width, int4 height, uint2 depth = 24);

    BMPImage(const BMPImage &image);

    BMPImage &operator=(const BMPImage &image);

    virtual ~BMPImage();

    bool create(int4 width,int4 height,uint2 depth=24);

    bool open(const char* bmp_file_path);

    bool save(const char* save_path);

    template <typename T>
    T & at(uint row, uint col);

    bool clear();

    int4 width();

    int4 height();

    uint2 depth();


private:
    int4 m_width;
    int4 m_height;
    uint2 m_depth;
    uint4 m_row_size;
    uchar *m_data;

    BMPFileHeader m_file_header;
    BMPInfoHeader m_info_header;
    uint m_color_palette_size;
    ColorPalette *m_color_palette;

	bool valid(int4 row,int4 col,uint2 depth);

};


template<typename T>
T &BMPImage::at(uint x, uint y) {
    size_t elem_size = sizeof(T);
#ifdef DEBUG
    if(!valid(x, y, (uint2)(elem_size*8))){ // TODO
        throw std::runtime_error("Index is overflow or T is not match depth");
    }
#endif
    T *value = nullptr;

    size_t index = 0;
	/*
    if (m_height > 0){
        // index = (m_height-y-1) * m_row_size + x*elem_size;
    }else{
        index = y * m_row_size + x * elem_size;
    }
	*/
	index = y * m_row_size + x * elem_size;
    value = (T*)(m_data+index);
    return *value;
}

#endif //BMPIMAGE_H
