#include "pixelpch.h"
#include "VertexArray.h"
#include "RendererCommands.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Pixel {
	std::shared_ptr<VertexArray> VertexArray::CreateVertexArray() {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLVertexArray>();
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}