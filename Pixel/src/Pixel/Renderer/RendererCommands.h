#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include <glm/glm.hpp>

namespace Pixel {
	class RendererCommands {
	public:
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
	};
}

#endif // !RENDERER_COMMANDS_H
