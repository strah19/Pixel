#include "pixelpch.h"
#include "Shader.h"

#include "Renderer/RendererCommands.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pixel {
	std::shared_ptr<Shader> Shader::CreateShader(const std::string& file_path) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLShader>(file_path);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}