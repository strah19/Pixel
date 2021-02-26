#include "pixelpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Pixel {
	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		glViewport(x, y, w, h);
	}

	void OpenGLRendererAPI::DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) {
		for (auto& i : vertex_array->GetVertexBuffers())
			glDrawElements(GL_TRIANGLES, i->GetLayout()->GetStride(), GL_UNSIGNED_INT, 0);
	}
}