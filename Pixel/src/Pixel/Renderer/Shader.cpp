#include "pixelpch.h"
#include "Shader.h"

#include "Renderer/RendererCommands.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pixel {
	std::shared_ptr<Shader> Shader::Create() {
		switch (RendererCommands::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLShader>();
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}