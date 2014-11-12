#pragma once

#include "ShaderProgram.hpp"

class ShaderProgramColor : public ShaderProgram
{
public:
	ShaderProgramColor(glm::mat4 *viewMatrix, glm::mat4 *projectonMatrix);

	virtual ~ShaderProgramColor();

	void draw(glm::mat4 *modelMatrix, GLuint vbo, glm::vec4 color, GLuint drawMode = GL_TRIANGLE_STRIP);

protected:
	glm::vec4 mColor;
};
