#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "Renderer/Shader.h"

namespace Pixel {
	class OpenGLShader : public Shader {
	public:
		virtual ~OpenGLShader();

		void Bind();
		void UnBind();

		void Init(const std::string& file_path);

		/* Uniforms go here! */
		void Set1f(const std::string& name, float& value);
		void SetMat4f(const std::string& name, const glm::mat4& mat4);

		uint32_t GetUniformLocation(const std::string& name);
		uint32_t GetId() const { return shader_id; }
	private:
		uint32_t shader_id;
		ShaderSources ParseShader(const std::string& path);
		uint32_t CompileShader(const std::string& source, uint32_t type);
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}

#endif // !OPENGL_SHADER_H
