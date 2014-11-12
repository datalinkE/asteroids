#pragma once

#include "ShaderProgram.hpp"

class ShaderProgramTexture : public ShaderProgram
{
public:
	ShaderProgramTexture(glm::mat4 *viewMatrix, glm::mat4 *projectonMatrix);

	virtual ~ShaderProgramTexture();

	void draw(glm::mat4 *modelMatrix, GLuint vbo, GLuint textureHandle, GLuint drawMode = GL_TRIANGLE_STRIP);

protected:
	GLuint mTextureHandle;
};
