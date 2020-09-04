#ifndef OPENGL_RENDERER_COMMANDS_H
#define OPENGL_RENDERER_COMMANDS_H

#include "Renderer/RendererCommands.h"

namespace Pixel {
	class OpenGLRendererCommands : public RendererCommands {
	public:
		void Clear();
		void SetClearColor(float r, float g, float b, float a);
		void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	};
}

#endif // !OPENGL_RENDERER_COMMANDS_H
