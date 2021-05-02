#ifndef OPENGL_CUBE_MAP_H
#define OPENGL_CUBE_MAP_H

#include "Renderer/CubeMap.h"
#include <glad/glad.h>

namespace Pixel {
	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(const std::vector<std::string>& faces);

		void Bind(uint32_t slot = 0);
		void UnBind();

		void Begin();
		void End();

		uint32_t GetTexture();
	private:
		uint32_t texture_id;
	};
}

#endif // !OPENGL_CUBE_MAP_H
