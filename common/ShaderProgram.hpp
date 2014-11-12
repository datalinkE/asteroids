#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "Logger.hpp"
#include "GLHelpers.h"

typedef std::map<std::string, GLuint> HandleMap;

class ShaderProgram
{
public:
	ShaderProgram(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix)
		: mViewMatrix(viewMatrix)
		, mProjectionMatrix(projectionMatrix)
		, mProgramHandle(0)
	{
		DLOG();
		// mProgramHandle should be set in derived classes
	}

	virtual ~ShaderProgram()
	{
		DLOG() << ARG(mProgramHandle);
	}

	void addAttribute(std::string name)
	{
	    mAttributes[name] = glGetAttribLocation(mProgramHandle, name.c_str());
	}

    void addUniform(std::string name)
    {
        mUniforms[name] = glGetUniformLocation(mProgramHandle, name.c_str());
    }

protected:
	glm::mat4 *mViewMatrix;
	glm::mat4 *mProjectionMatrix;
	GLuint mProgramHandle;
	HandleMap mAttributes;
	HandleMap mUniforms;

};
