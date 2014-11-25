#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <map>
#include <string>

#include "platform_gl.h"

class FontAtlas
{
public:
    struct CharacterInfo
    {
      float advanceX; // advance.x
      float advanceY; // advance.y

      float bmpWidth; // bitmap.width;
      float bmpHeight; // bitmap.rows;

      float bmpLeft; // bitmap_left;
      float bmptop; // bitmap_top;

      float txOffset; // x offset of glyph in texture coordinates
    };

public:
    FontAtlas(const char* fontName, int fontSize);
    virtual ~FontAtlas();

    GLuint handle() const;

    const CharacterInfo& charInfo(char c) const;

    unsigned width() const;
    unsigned height() const;
private:
    static FT_Library sFreetypeGlobal;
    static bool sInited;
    static void initFreetype();

private:
    std::string mFontName;
    int mFontSize;

    FT_Face mFace;
    unsigned mAtlasWidth;
    unsigned mAtlasHeight;

    GLuint mAtlasHandle;
    std::map<char, CharacterInfo> mChars;
};
