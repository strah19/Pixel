#include "pixelpch.h"
#include "RendererCommands.h"

namespace Pixel {
	static std::shared_ptr<RendererAPI> renderer_api;

	void RendererCommand::Init() {
		renderer_api = RendererAPI::CreateRendererAPI();
		renderer_api->Init();
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { 
		renderer_api->SetViewport(x, y, w, h); 	
	}
	void RendererCommand::Clear() { 
		renderer_api->Clear(); 
	}

	void RendererCommand::SetClearColor(float r, float g, float b, float a) { 
		renderer_api->SetClearColor(r, g, b, a);
	}

	void RendererCommand::DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) {
		renderer_api->DrawVertexArray(vertex_array);
	}
}