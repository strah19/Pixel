#include "pixelpch.h"
#include "RendererContext.h"

#include "Renderer/RendererCommands.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Pixel {
	std::unique_ptr<RendererContext> RendererContext::CreateRendererContext(Window* window) {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window->GetNativeWindow()));
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}