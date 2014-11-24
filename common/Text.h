#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "platform_gl.h"

#include "ShaderProgram.hpp"

#include <map>
#include <string>
#include <glm/glm.hpp>

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

class Text : public ShaderProgram
{
public:
    Text(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix);
    virtual ~Text();

    void render(const std::string& text, glm::mat4* modelMatrix, float sx = 1.0f, float sy = 1.0f);

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

    GLuint mVbo;
};

