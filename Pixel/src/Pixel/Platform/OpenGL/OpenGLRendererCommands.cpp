#include "pixelpch.h"
#include "OpenGLRendererCommands.h"

#include <glad/glad.h>

namespace Pixel {
	void OpenGLRendererCommands::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererCommands::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererCommands::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		glViewport(x, y, w, h);
	}
}