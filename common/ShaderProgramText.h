#pragma once

#include "platform_gl.h"

#include "ShaderProgram.hpp"
#include "FontAtlas.h"

#include <string>
#include <glm/glm.hpp>

#include "FontAtlas.h"


class ShaderProgramText : public ShaderProgram
{
public:
    ShaderProgramText(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix);
    virtual ~ShaderProgramText();

    void draw(const std::string& text, glm::mat4* modelMatrix, const FontAtlas& mFontAtlas);

private:
    GLuint mVbo;
};

