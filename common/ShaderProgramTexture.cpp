#include "ShaderProgramTexture.h"

using namespace glm;
using namespace GLHelpers;

ShaderProgramTexture::ShaderProgramTexture(mat4 *viewMatrix, mat4 *projectionMatrix, GLuint textureHandle)
	: ShaderProgram(viewMatrix, projectionMatrix)
	, mTextureHandle(textureHandle)
{
	DLOG();
	mProgramHandle = build_program_from_assets("Texture.vsh", "Texture.fsh");
	ma_PositionHandle = glGetAttribLocation(mProgramHandle, "a_Position");
	ma_TextureCoordinatesHandle = glGetAttribLocation(mProgramHandle, "a_TextureCoordinates");
	mu_TextureUnitHandle = glGetUniformLocation(mProgramHandle, "u_TextureUnit");
	mu_MVPMatrixHandle = glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
}

ShaderProgramTexture::~ShaderProgramTexture()
{
	DLOG();
}

void ShaderProgramTexture::draw(mat4 *modelMatrix, GLuint vbo, GLuint drawMode)
{
		glUseProgram(mProgramHandle);

		int vertexDataSize = 4 * sizeof(GL_FLOAT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureHandle);

		mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
		glUniformMatrix4fv(mu_MVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

		glUniform1i(mu_TextureUnitHandle, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		int vboSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);

		glVertexAttribPointer(ma_PositionHandle, 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(0));
	    glEnableVertexAttribArray(ma_PositionHandle);

		glVertexAttribPointer(ma_TextureCoordinatesHandle, 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(ma_TextureCoordinatesHandle);

		glDrawArrays(drawMode, 0, vboSize / vertexDataSize);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
}
