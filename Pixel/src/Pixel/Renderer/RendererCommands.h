#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include <memory>

namespace Pixel {
	enum class RenderAPI {
		OpenGL, None
	};

	class RendererCommands {
	public:
		virtual void Clear() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

		static std::shared_ptr<RendererCommands> CreateRendererCommands();
		static RenderAPI GetAPI() { return API; }
	private:
		static RenderAPI API;
	};
}

#endif // !RENDERER_COMMANDS_H
