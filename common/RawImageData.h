#pragma once

#include "platform_gl.h"

class RawImageData
{
public:
    int width;
    int height;
    int size;
    GLenum gl_color_format;
    const unsigned char* data;

public:
    RawImageData(const void* png_data, const int png_data_size);

    ~RawImageData();
};
