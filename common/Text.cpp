#include "Text.h"
#include "Logger.hpp"
#include "GLHelpers.h"
#include "platform_asset.h"
#include <string>

bool Text::sInited(false);
FT_Library Text::sFreetypeGlobal;

void Text::initFreetype()
{
    DLOG();
    int error = FT_Init_FreeType(&sFreetypeGlobal);
    if (error)
    {
        DLOG() << error;
        CRASH("Freetype initialisation failed!");
    }
    sInited = true;
}

Text::Text(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix)
	: mAtlasWidth(0)
	, mAtlasHeight(0)
	, ShaderProgram(viewMatrix, projectionMatrix)
{
    if (!sInited)
    {
    	initFreetype();
    }


	mProgramHandle = GLHelpers::build_program_from_assets("Text.vsh", "Text.fsh");

	addAttribute("coord");
	addUniform("tex");
	addUniform("color");


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

    DLOG() << "font loaded";

    FT_Set_Pixel_Sizes(mFace, 0, 48);
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
    glUniform1i(mUniforms["tex"], 0);

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

      info.ax = g->advance.x / 64; // glyph value in 1/64 pixels

      DLOG() << "boom";

      info.ay = g->advance.y / 64;

      info.bw = g->bitmap.width;
      info.bh = g->bitmap.rows;

      info.bl = g->bitmap_left;
      info.bt = g->bitmap_top;

      info.tx = (float)x / mAtlasWidth;
      x += g->bitmap.width;

      DLOG() << "char" << (char)c << "in atlas";
    }
    DLOG() << "done creating atlas";

    glGenBuffers(1, &mVbo);
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

void Text::render(const std::string& text, glm::mat4* modelMatrix, float sx, float sy)
{
    glUseProgram(mProgramHandle);

    glm::mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
    glUniformMatrix4fv(mUniforms["u_MVPMatrix"], 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    GLfloat red[4] = { 1, 0, 0, 1 };
    glUniform4fv(mUniforms["color"], 1, red);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, mAtlasHandle);
    glUniform1i(mUniforms["tex"], 0);

    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(mAttributes["coord"]);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glVertexAttribPointer(mAttributes["coord"], 4, GL_FLOAT, GL_FALSE, 0, 0);


	point coords[6 * text.length()];

	int n = 0;

	float x = 0;
	float y = 0;

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
	glDisableVertexAttribArray(mAttributes["coord"]);
}
