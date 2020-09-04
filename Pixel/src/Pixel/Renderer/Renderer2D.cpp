#include "pixelpch.h"
#include "Renderer2D.h"

#include "Renderer/RendererCommands.h"
#include "Platform/OpenGL/OpenGLRenderer2D.h"

namespace Pixel {
	std::shared_ptr<Renderer2D> Renderer2D::CreateRenderer() {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpeGLRenderer2D>();
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}