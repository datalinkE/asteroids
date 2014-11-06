#include "GLHelpers.h"

#include <assert.h>
#include "Logger.hpp"

namespace GLHelpers
{
	GLuint createVBO(const GLsizeiptr size, const GLvoid* data, const GLenum usage)
	{
	    DLOG();
	    assert(data != NULL);
		GLuint vboObject;
		glGenBuffers(1, &vboObject);
		assert(vboObject != 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboObject);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return vboObject;
	}
}
