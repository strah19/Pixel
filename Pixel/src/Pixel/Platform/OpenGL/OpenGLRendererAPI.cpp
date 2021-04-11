#include "pixelpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Pixel {
	void OpenGLRendererAPI::Init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		glViewport(x, y, w, h);
	}

	void OpenGLRendererAPI::DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) {
		glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	void OpenGLRendererAPI::DrawVertexArrayInstanced(std::shared_ptr<VertexArray> vertex_array, uint32_t instance_count) {
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_array->GetIndexBufferSize(), instance_count);
	}

	void OpenGLRendererAPI::DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride) {
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, indirect, count, stride);
	}
}