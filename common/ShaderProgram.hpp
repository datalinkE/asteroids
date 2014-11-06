#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Logger.hpp"
#include "GLHelpers.h"

class ShaderProgram
{
public:
	ShaderProgram(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
		: mViewMatrix(viewMatrix)
		, mProjectionMatrix(projectionMatrix)
		, mProgramHandle(0)
	{
		DLOG();
	}

	virtual ~ShaderProgram()
	{
		DLOG() << ARG(mProgramHandle);
	}

	virtual void draw(glm::mat4 &modelMatrix, GLuint vbo) = 0;

protected:
	glm::mat4 &mViewMatrix;
	glm::mat4 &mProjectionMatrix;
	glm::mat4 mMVPMatrix;
	GLuint mProgramHandle;
	GLuint ma_PositionHandle;
};
