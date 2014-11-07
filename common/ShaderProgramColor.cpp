#include "ShaderProgramColor.h"

using namespace glm;
using namespace GLHelpers;

ShaderProgramColor::ShaderProgramColor(mat4 *viewMatrix, mat4 *projectionMatrix, vec4 color)
	: ShaderProgram(viewMatrix, projectionMatrix)
	, mColor(color)
{
	DLOG();
	mProgramHandle = build_program_from_assets("SolidColor.vsh", "SolidColor.fsh");
	ma_PositionHandle = glGetAttribLocation(mProgramHandle, "a_Position");
	mu_ColorHandle = glGetUniformLocation(mProgramHandle, "u_Color");
	mu_MVPMatrixHandle = glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
}

ShaderProgramColor::~ShaderProgramColor()
{
	DLOG();
}

void ShaderProgramColor::draw(mat4 *modelMatrix, GLuint vbo, GLuint drawMode)
{
	glUseProgram(mProgramHandle);

	int vertexDataSize = 4 * sizeof(GL_FLOAT);

	mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
	glUniformMatrix4fv(mu_MVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

	glUniform4fv(mu_ColorHandle, 1,glm::value_ptr(mColor));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	int vboSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);

	glVertexAttribPointer(ma_PositionHandle, 2, GL_FLOAT, GL_FALSE, vertexDataSize, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(ma_PositionHandle);

	glDrawArrays(drawMode, 0, vboSize / vertexDataSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
