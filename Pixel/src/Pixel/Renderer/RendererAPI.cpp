#include "pixelpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Pixel {
	RenderAPI RendererAPI::API = RenderAPI::OpenGL;

	std::shared_ptr<RendererAPI> RendererAPI::CreateRendererAPI() {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLRendererAPI>();
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}