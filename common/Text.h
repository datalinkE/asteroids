#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

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

private:
    static FT_Library sFreetypeGlobal;
    static bool sInited;
    static void initFreetype();

private:
    FT_Face mFace;
    unsigned mAtlasWidth;
    unsigned mAtlasHeight;
    std::map<char, CharacterInfo> mChars;
};

