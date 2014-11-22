#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "platform_gl.h"

#include <map>
#include <string>

struct CharacterInfo
{
  float ax; // advance.x
  float ay; // advance.y

  float bw; // bitmap.width;
  float bh; // bitmap.rows;

  float bl; // bitmap_left;
  float bt; // bitmap_top;

  float tx; // x offset of glyph in texture coordinates
};

class Text
{
public:
    Text();
    virtual ~Text();

    void render(const std::string& text, float x, float y, float sx = 1.0f, float sy = 1.0f);

private:
    static FT_Library sFreetypeGlobal;
    static bool sInited;
    static void initFreetype();

private:
    FT_Face mFace;
    unsigned mAtlasWidth;
    unsigned mAtlasHeight;

    GLuint mAtlasHandle;
    std::map<char, CharacterInfo> mChars;
};

