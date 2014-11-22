#include "Text.h"
#include "Logger.hpp"
#include "platform_asset.h"
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

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mAtlasHandle);
    glBindTexture(GL_TEXTURE_2D, mAtlasHandle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mAtlasWidth, mAtlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

    int x = 0;

    for(int c = 32; c < 128; c++)
    {
      if(FT_Load_Char(mFace, c, FT_LOAD_RENDER))
        continue;

      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

      CharacterInfo& info = mChars.at(c);
      info.ax = g->advance.x >> 6; // glyph value in 1/64 pixels
      info.ay = g->advance.y >> 6;

      info.bw = g->bitmap.width;
      info.bh = g->bitmap.rows;

      info.bl = g->bitmap_left;
      info.bt = g->bitmap_top;

      info.tx = (float)x / mAtlasWidth;
      x += g->bitmap.width;
    }
}

Text::~Text() {
    // TODO Auto-generated destructor stub
}

struct point
{
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
};

void Text::render(const std::string& text, float x, float y, float sx, float sy)
{
	point coords[6 * text.length()];

	int n = 0;

	for (const char& c : text)
	{
		CharacterInfo& info = mChars.at(c);
		float x2 =  x + info.bl * sx;
		float y2 = -y - info.bt * sy;
		float w = info.bw * sx;
		float h = info.bh * sy;

		/* Advance the cursor to the start of the next character */
		x += info.ax * sx;
		y += info.ay * sy;

		/* Skip glyphs that have no pixels */
		if(!w || !h)
			continue;

		coords[n++] = (point){x2, -y2,
							 info.tx, 0};
		coords[n++] = (point){x2 + w, -y2,
							 info.tx + info.bw / mAtlasWidth, 0};
		coords[n++] = (point){x2, -y2 - h,
							 info.tx, info.bh / mAtlasHeight }; //remember: each glyph occupies a different amount of vertical space
		coords[n++] = (point){x2 + w, -y2,
							 info.tx + info.bw / mAtlasWidth, 0};
		coords[n++] = (point){x2,     -y2 - h,
							 info.tx, info.bh / mAtlasHeight};
		coords[n++] = (point){x2 + w, -y2 - h,
							 info.tx + info.bw / mAtlasWidth, info.bh / mAtlasHeight};
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, n);
}
