#ifndef RENDERER_API_H
#define RENDERER_API_H

#include <memory>
#include "Renderer/VertexArray.h"

namespace Pixel {	
	enum class RenderAPI {
		OpenGL, None
	};

	class RendererAPI {
	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
		virtual void DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) = 0;
		virtual void DrawVertexArrayInstanced(std::shared_ptr<VertexArray> vertex_array, uint32_t instance_count) = 0;

		static std::shared_ptr<RendererAPI> CreateRendererAPI();
		inline static RenderAPI GetAPI() { return API; }
	private:
		static RenderAPI API;
	};
}

#endif // !RENDERER_API_H
