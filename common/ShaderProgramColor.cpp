#include "ShaderProgramColor.h"

using namespace glm;
using namespace GLHelpers;

ShaderProgramColor::ShaderProgramColor(mat4 &viewMatrix, mat4 &projectionMatrix, vec4 color)
	: ShaderProgram(viewMatrix, projectionMatrix)
	, mColor(color)
{
	DLOG();
	mProgramHandle = build_program_from_assets("BasicSolidColor.vsh", "BasicSolidColor.fsh");
	ma_PositionHandle = glGetAttribLocation(mProgramHandle, "a_Position");
	mu_ColorHandle = glGetUniformLocation(mProgramHandle, "u_Color");
}

ShaderProgramColor::~ShaderProgramColor()
{
	DLOG();
}

void ShaderProgramColor::draw(mat4 &modelMatrix, GLuint vbo)
{
	glUseProgram(mProgramHandle);

	glUniform4fv(mu_ColorHandle, 1,glm::value_ptr(mColor));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(ma_PositionHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(ma_PositionHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
