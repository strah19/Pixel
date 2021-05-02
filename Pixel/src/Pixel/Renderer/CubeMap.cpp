#include "pixelpch.h"
#include "Platform/OpenGL/OpenGLCubeMap.h"
#include "Renderer/RendererCommands.h"

namespace Pixel {
	std::shared_ptr<CubeMap> CubeMap::CreateCubeMap(const std::vector<std::string>& faces) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLCubeMap>(faces);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}