#include "pixelpch.h"
#include "RendererCommands.h"

#include "Platform/OpenGL/OpenGLRendererCommands.h"

namespace Pixel {
	RenderAPI RendererCommands::API = RenderAPI::OpenGL;

	std::shared_ptr<RendererCommands> RendererCommands::CreateRendererCommands() {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLRendererCommands>();
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}