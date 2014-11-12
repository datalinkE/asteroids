#include "ShaderProgramTexture.h"

using namespace glm;
using namespace GLHelpers;

ShaderProgramTexture::ShaderProgramTexture(mat4 *viewMatrix, mat4 *projectionMatrix)
	: ShaderProgram(viewMatrix, projectionMatrix)
{
	DLOG();
	mProgramHandle = build_program_from_assets("Texture.vsh", "Texture.fsh");

	addAttribute("a_Position");
	addAttribute("a_TextureCoordinates");

	addUniform("u_TextureUnit");
	addUniform("u_MVPMatrix");
}

ShaderProgramTexture::~ShaderProgramTexture()
{
	DLOG();
}

void ShaderProgramTexture::draw(mat4 *modelMatrix, GLuint vbo, GLuint textureHandle, GLuint drawMode)
{
		glUseProgram(mProgramHandle);

		int vertexDataSize = 4 * sizeof(GL_FLOAT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
		glUniformMatrix4fv(mUniforms["u_MVPMatrix"], 1, GL_FALSE, glm::value_ptr(MVPMatrix));

		glUniform1i(mUniforms["u_TextureUnit"], 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		int vboSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);

		glVertexAttribPointer(mAttributes["a_Position"], 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(0));
	    glEnableVertexAttribArray(mAttributes["a_Position"]);

		glVertexAttribPointer(mAttributes["a_TextureCoordinates"], 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(mAttributes["a_TextureCoordinates"]);

		glDrawArrays(drawMode, 0, vboSize / vertexDataSize);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
}
