#pragma once

#include "platform_gl.h"

namespace GLHelpers
{
	GLuint createVBO(const GLsizeiptr size, const GLvoid* data, const GLenum usage);

	GLuint build_program_from_assets(const char* vertex_shader_path, const char* fragment_shader_path);

	/* Should be called just before using a program to draw, if validation is needed. */
	GLint validate_program(const GLuint program);
}
