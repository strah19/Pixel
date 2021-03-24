#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <memory>
#include <glm.hpp>

namespace Pixel {
	struct ShaderSources
	{
		std::string vertex;
		std::string fragment;
	};

	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Init(const std::string& file_path) = 0;

		static std::shared_ptr<Shader> CreateShader();

		/* Uniforms go here! */
		virtual void Set1f(const std::string& name, float& value) = 0;
		virtual void SetMat4f(const std::string& name, const glm::mat4& mat4) = 0;

		virtual uint32_t GetId() const = 0;
		virtual uint32_t GetUniformLocation(const std::string& name) = 0;
	private:
	};
}

#endif // !SHADER_H
