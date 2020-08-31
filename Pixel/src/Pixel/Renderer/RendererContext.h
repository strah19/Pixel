#ifndef RENDERER_CONTEXT_H
#define RENDERER_CONTEXT_H

#include <memory>

#include "RendererAPI.h"
#include "Platform/Window.h"

namespace Pixel {
	class RendererContext {
	public:
		virtual ~RendererContext() = 0;

		virtual void SwapBuffers() = 0;
		virtual void SetVSync(int interval) = 0;

		static std::unique_ptr<RendererContext> CreateRendererContext(Window* window);
	private:

	};
}

#endif // !RENDERER_CONTEXT_H
