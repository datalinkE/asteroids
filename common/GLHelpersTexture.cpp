#include <assert.h>
#include "GLHelpers.h"
#include "Logger.hpp"
#include "RawImageData.h"
#include "platform_asset.h"

namespace GLHelpers
{
	GLuint load_texture(
		const GLsizei width, const GLsizei height,
		const GLenum type, const GLvoid* pixels)
	{
		DLOG();
		GLuint texture_object_id;
		glGenTextures(1, &texture_object_id);
		assert(texture_object_id != 0);

		glBindTexture(GL_TEXTURE_2D, texture_object_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		return texture_object_id;
	}

	GLuint load_png_asset_into_texture(const char* relative_path)
	{
		assert(relative_path != NULL);
		DLOG() << relative_path;

		const std::string png_file = get_asset_data(relative_path);
		const RawImageData raw_image_data(png_file.data(), png_file.length());
		const GLuint texture_object_id = load_texture(
			raw_image_data.width, raw_image_data.height,
			raw_image_data.gl_color_format, raw_image_data.data);

		return texture_object_id;
	}
}


