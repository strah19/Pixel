#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Renderer/Texture.h"

namespace Pixel {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const char* file_path);

		void Bind();
		void UnBind();

	private:
		uint32_t texture_id;
	};
}

#endif // !OPENGL_TEXTURE_H
