#include "Text.h"
#include "Logger.hpp"
#include "platform_asset.h"
#include "platform_gl.h"
#include <string>

bool Text::sInited(false);
FT_Library Text::sFreetypeGlobal;

void Text::initFreetype()
{
    int error = FT_Init_FreeType(&sFreetypeGlobal);
    if (error)
    {
        DLOG() << error;
        CRASH("Freetype initialisation failed!");
    }
    sInited = true;
}

Text::Text()
	: mAtlasWidth(0)
	, mAtlasHeight(0)
{
    if (!sInited)
    {
    	initFreetype();
    }

    std::string fontData = get_asset_data("Reckoner.ttf");
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

    FT_Set_Pixel_Sizes(mFace, 0, 48);
    FT_GlyphSlot g = mFace->glyph;

    unsigned mAtlasWidth = 0;
    unsigned mAtlasHeight = 0;

    for(int i = 32; i < 128; i++)
    {
    	if(FT_Load_Char(mFace, i, FT_LOAD_RENDER))
    	{
    		DLOG() << "Loading character" << i << "failed!";
    		continue;
    	}
    	mAtlasWidth += g->bitmap.width;
    	mAtlasHeight = std::max(mAtlasHeight, g->bitmap.rows);
    }

    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mAtlasWidth, mAtlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

    int x = 0;

    for(int i = 32; i < 128; i++)
    {
      if(FT_Load_Char(mFace, i, FT_LOAD_RENDER))
        continue;

      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

      x += g->bitmap.width;
    }
}

Text::~Text() {
    // TODO Auto-generated destructor stub
}

