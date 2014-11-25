#include "FontAtlas.h"

#include "Logger.hpp"
#include "GLHelpers.h"
#include "platform_asset.h"
#include <string>

bool FontAtlas::sInited(false);
FT_Library FontAtlas::sFreetypeGlobal;

void FontAtlas::initFreetype()
{
    DLOG();

    if(sInited)
    {
        return;
    }

    int error = FT_Init_FreeType(&sFreetypeGlobal);
    if (error)
    {
        DLOG() << error;
        CRASH("Freetype initialisation failed!");
    }
    sInited = true;
}

FontAtlas::FontAtlas(const char* fontName, int fontSize)
    : mFontName(fontName)
    , mFontSize(fontSize)
    , mAtlasWidth(0)
    , mAtlasHeight(0)
{
    initFreetype();

    std::string fontData = get_asset_data(fontName);
    int error = FT_New_Memory_Face( sFreetypeGlobal,
                                reinterpret_cast<FT_Byte const*>(fontData.data()),    /* first byte in memory */
                                fontData.size(),      /* size in bytes        */
                                0,         /* face_index           */
                                &mFace );
    if (error)
    {
        DLOG() << error;
        CRASH("Font loading failed!");
    }

    DLOG() << "font loaded";

    FT_Set_Pixel_Sizes(mFace, 0, mFontSize);
    FT_GlyphSlot g = mFace->glyph;

    for(int c = 32; c < 128; c++)
    {
        if(FT_Load_Char(mFace, c, FT_LOAD_RENDER))
        {
            DLOG() << "Loading character" << c << "failed!";
            continue;
        }
        mAtlasWidth += g->bitmap.width;
        mAtlasHeight = std::max(mAtlasHeight, g->bitmap.rows);
    }

    DLOG() << "atlas width is" << mAtlasWidth;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mAtlasHandle);
    glBindTexture(GL_TEXTURE_2D, mAtlasHandle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mAtlasWidth, mAtlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    DLOG() << "before second loop";

    int x = 0;
    for(int c = 32; c < 128; c++)
    {
      if(FT_Load_Char(mFace, c, FT_LOAD_RENDER))
        continue;

      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

      DLOG() << "before iterator";
      CharacterInfo& info = mChars[c];
      DLOG() << "after iterator";

      info.advanceX = g->advance.x / 64; // glyph value in 1/64 pixels

      DLOG() << "boom";

      info.advanceY = g->advance.y / 64;

      info.bmpWidth = g->bitmap.width;
      info.bmpHeight = g->bitmap.rows;

      info.bmpLeft = g->bitmap_left;
      info.bmptop = g->bitmap_top;

      info.txOffset = (float)x / mAtlasWidth;
      x += g->bitmap.width;

      DLOG() << "char" << (char)c << "in atlas";
    }
    DLOG() << "done creating atlas";
}

FontAtlas::~FontAtlas()
{
    // TODO Auto-generated destructor stub
}

GLuint FontAtlas::handle() const
{
    return mAtlasHandle;
}

const FontAtlas::CharacterInfo& FontAtlas::charInfo(char c) const
{
    return mChars.at(c);
}

unsigned FontAtlas::width() const
{
    return mAtlasWidth;
}

unsigned FontAtlas::height() const
{
    return mAtlasHeight;
}

