#include "ShaderProgramColor.h"

using namespace glm;
using namespace GLHelpers;

ShaderProgramColor::ShaderProgramColor(mat4 *viewMatrix, mat4 *projectionMatrix)
	: ShaderProgram(viewMatrix, projectionMatrix)
{
	DLOG();
	mProgramHandle = build_program_from_assets("SolidColor.vsh", "SolidColor.fsh");

	addAttribute("a_Position");

	addUniform("u_Color");
	addUniform("u_MVPMatrix");
}

ShaderProgramColor::~ShaderProgramColor()
{
	DLOG();
}

void ShaderProgramColor::draw(mat4 *modelMatrix, GLuint vbo, vec4 color, GLuint drawMode)
{
	glUseProgram(mProgramHandle);

	int vertexDataSize = 4 * sizeof(GL_FLOAT);

	mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
	glUniformMatrix4fv(mUniforms["u_MVPMatrix"], 1, GL_FALSE, glm::value_ptr(MVPMatrix));

	glUniform4fv(mUniforms["u_Color"], 1, glm::value_ptr(color));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	int vboSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);

	glVertexAttribPointer(mAttributes["a_Position"], 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(mAttributes["a_Position"]);

	glDrawArrays(drawMode, 0, vboSize / vertexDataSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
