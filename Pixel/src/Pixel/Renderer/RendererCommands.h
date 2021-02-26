#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include <memory>
#include "RendererAPI.h"

namespace Pixel {
	class RendererCommand {
	public:
		static void Init();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		static void Clear();
		static void SetClearColor(float r, float g, float b, float a);
		static void DrawVertexArray(std::shared_ptr<VertexArray> vertex_array); 
	};
}

#endif // !RENDERER_COMMANDS_H
