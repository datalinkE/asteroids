#include "GLHelpers.h"

namespace GLHelpers
{
	GLuint createVBO(const GLsizeiptr size, const GLvoid* data, const GLenum usage) {
		assert(data != NULL);
		GLuint vboObject;
		glGenBuffers(1, &vboObject);
		assert(vbo_object != 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboObject);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return vboObject;
	}
}