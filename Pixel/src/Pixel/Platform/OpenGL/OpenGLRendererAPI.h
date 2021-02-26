#ifndef OPENGL_RENDERER_COMMANDS_H
#define OPENGL_RENDERER_COMMANDS_H

#include "Renderer/RendererAPI.h"

namespace Pixel {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		void Clear();
		void SetClearColor(float r, float g, float b, float a);
		void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		void DrawVertexArray(std::shared_ptr<VertexArray> vertex_array);
	};
}

#endif // !OPENGL_RENDERER_COMMANDS_H
