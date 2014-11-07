#pragma once

#include "ShaderProgram.hpp"

class ShaderProgramTexture : public ShaderProgram
{
public:
	ShaderProgramTexture(glm::mat4 *viewMatrix, glm::mat4 *projectonMatrix, GLuint textureHandle);

	virtual ~ShaderProgramTexture();

	virtual void draw(glm::mat4 *modelMatrix, GLuint vbo, GLuint drawMode);

protected:
	GLuint mTextureHandle;
	GLuint ma_TextureCoordinatesHandle;
	GLuint mu_TextureUnitHandle;
};
