#include "pixelpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Pixel {
	static uint32_t current_texture_id = 0;

	OpenGLTexture::OpenGLTexture(const char* file_path) {
		path = file_path;
		stbi_set_flip_vertically_on_load(1);
		int w, h, channels;
		unsigned char* data = stbi_load(file_path, &w, &h, &channels, 0);
		width = w;
		height = h;
		if (channels == 4) {
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (channels == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}

		if (data) {
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
			glTextureStorage2D(texture_id, 1, internal_format, width, height);

			glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture: " << file_path << std::endl;
		
		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height) {
		internal_format = GL_RGBA8;
		data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
		glTextureStorage2D(texture_id, 1, internal_format, width, height);

		glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::~OpenGLTexture() {
		glDeleteTextures(1, &texture_id);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size) {
		uint32_t bpp = data_format == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) {
		if (current_texture_id != texture_id) {
			glBindTextureUnit(slot, texture_id);
			current_texture_id = texture_id;
		}
	}

	void OpenGLTexture::UnBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

