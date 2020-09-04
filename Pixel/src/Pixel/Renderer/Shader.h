#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <memory>

namespace Pixel {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Init(const char* vertex_shader, const char* fragment_shader) = 0;

		static std::shared_ptr<Shader> CreateShader();

		/* Uniforms go here! */
		virtual void Set1f(const std::string& name, float& value) = 0;

		virtual uint32_t GetUniformLocation(const std::string& name) = 0;
	private:

	};
}

#endif // !SHADER_H
