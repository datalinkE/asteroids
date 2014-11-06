#include "GLHelpers.h"

#include <string>
#include <assert.h>
#include "Logger.hpp"
#include "platform_asset.h"

namespace GLHelpers
{
	std::string shader_info_log(GLuint shader_object_id)
	{
	    DLOG() << ARG(shader_object_id);
		GLint log_length;
		glGetShaderiv(shader_object_id, GL_INFO_LOG_LENGTH, &log_length);

		DLOG() << ARG(log_length);
		if (!log_length)
		{
			return std::string();
		}

		GLchar log_buffer[log_length];
		glGetShaderInfoLog(shader_object_id, log_length, NULL, log_buffer);
		return std::string(log_buffer, log_length);
	}

	std::string program_info_log(GLuint program_object_id)
	{
	    DLOG() << ARG(program_object_id);
		GLint log_length;
		glGetProgramiv(program_object_id, GL_INFO_LOG_LENGTH, &log_length);

		DLOG() << ARG(log_length);
		if (!log_length)
		{
			return std::string();
		}

		GLchar log_buffer[log_length];
		glGetProgramInfoLog(program_object_id, log_length, NULL, log_buffer);
		return std::string(log_buffer, log_length);
	}

	GLuint compile_shader(const GLenum type, const GLchar* source, const GLint length)
	{
	    DLOG();
	    assert(source != NULL);
	    GLuint shader_object_id = glCreateShader(type);
	    GLint compile_status;

	    assert(shader_object_id != 0);

	    glShaderSource(shader_object_id, 1, (const GLchar **)&source, &length);
	    glCompileShader(shader_object_id);
	    glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);

		DLOG() << "Results of compiling shader source:" << std::endl
			   << std::string(source, length) << std::endl
			   << ARG(compile_status)
		       << ARG(shader_info_log(shader_object_id));

	    assert(compile_status != 0);

	    return shader_object_id;
	}

	GLuint link_program(const GLuint vertex_shader, const GLuint fragment_shader)
	{
	    DLOG();
	    GLuint program_object_id = glCreateProgram();
	    GLint link_status;

	    assert(program_object_id != 0);

	    glAttachShader(program_object_id, vertex_shader);
	    glAttachShader(program_object_id, fragment_shader);
	    glLinkProgram(program_object_id);
	    glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

	    DLOG() << "Results of linking program:" << std::endl
	    	   << ARG(link_status)
	           << ARG(program_info_log(program_object_id));

	    assert(link_status != 0);

	    return program_object_id;
	}

	GLuint build_program(
	    const GLchar * vertex_shader_source, const GLint vertex_shader_source_length,
	    const GLchar * fragment_shader_source, const GLint fragment_shader_source_length)
	{
	    DLOG();
	    assert(vertex_shader_source != NULL);
	    assert(fragment_shader_source != NULL);

	    GLuint vertex_shader = compile_shader(
	        GL_VERTEX_SHADER, vertex_shader_source, vertex_shader_source_length);
	    GLuint fragment_shader = compile_shader(
	        GL_FRAGMENT_SHADER, fragment_shader_source, fragment_shader_source_length);
	    return link_program(vertex_shader, fragment_shader);
	}

	GLint validate_program(const GLuint program)
	{
	    DLOG();
        int validate_status;
        glValidateProgram(program);
        glGetProgramiv(program, GL_VALIDATE_STATUS, &validate_status);

        DLOG() << ARG(validate_status);
        DLOG() << ARG(program_info_log(program));

        return validate_status;
	}

	GLuint build_program_from_assets(const char* vertex_shader_path, const char* fragment_shader_path)
	{
		DLOG();
	    assert(vertex_shader_path != NULL);
	    assert(fragment_shader_path != NULL);

	    const std::string vertex_shader_source = get_asset_data(vertex_shader_path);
	    const std::string fragment_shader_source = get_asset_data(fragment_shader_path);
	    const GLuint program_object_id = build_program(
	        vertex_shader_source.data(), vertex_shader_source.length(),
	        fragment_shader_source.data(), fragment_shader_source.length());

	    return program_object_id;
	}
}

