#include "pixelpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Pixel {
	OpenGLTexture::OpenGLTexture(const char* file_path) {
		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
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

	void OpenGLTexture::SetData(void* data, uint32_t size) {
		uint32_t bpp = data_format == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	void OpenGLTexture::UnBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
