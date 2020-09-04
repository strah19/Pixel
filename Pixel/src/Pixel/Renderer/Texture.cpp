#include "pixelpch.h"
#include "Texture.h"

#include "Renderer/RendererCommands.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pixel {
	std::shared_ptr<Texture> Texture::CreateTexture(const char* file_path) {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLTexture>(file_path);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}