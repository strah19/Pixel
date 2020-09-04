#include "pixelpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Pixel {
	OpenGLTexture::OpenGLTexture(const char* file_path) {
		glGenTextures(1, &texture_id);
	}

	void OpenGLTexture::Bind() {
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	void OpenGLTexture::UnBind() {

	}

}

