#include "pixelpch.h"
#include "Renderer2D.h"

#include "Renderer/RendererCommands.h"

namespace Pixel {
	void Renderer::BeginScene() {

	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(std::shared_ptr<VertexArray> vertex_array, std::shared_ptr<Shader> shader) {
		shader->Bind();
		vertex_array->Bind();
		vertex_array->GetIndexBuffers()->Bind();

		Pixel::RendererCommand::DrawVertexArray(vertex_array);
	}
}