#pragma once

#include "ShaderProgram.hpp"

class ShaderProgramColor : public ShaderProgram
{
public:
	ShaderProgramColor(glm::mat4 &viewMatrix, glm::mat4 &projectonMatrix, glm::vec4 color);

	virtual ~ShaderProgramColor();

	virtual void draw(glm::mat4 &modelMatrix, GLuint vbo);

protected:
	glm::vec4 mColor;
	GLuint mu_ColorHandle;
};
