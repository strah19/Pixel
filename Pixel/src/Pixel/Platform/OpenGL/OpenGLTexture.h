#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Renderer/Texture.h"
#include <glad/glad.h>

namespace Pixel {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const char* file_path);
		OpenGLTexture(uint32_t width, uint32_t height);

		virtual ~OpenGLTexture();

		void Bind(uint32_t slot = 0);
		void UnBind();

		virtual void SetData(void* data, uint32_t size);

		virtual uint32_t GetWidth() const { return width; }
		virtual uint32_t GetHeight() const { return height; }
		virtual uint32_t GetTextureId() const { return texture_id; }
	private:
		uint32_t texture_id;

		uint32_t width = 0;
		uint32_t height = 0;
		GLenum internal_format = 0, data_format = 0;
	};
}

#endif // !OPENGL_TEXTURE_H
