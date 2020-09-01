#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "Renderer/Shader.h"

namespace Pixel {
	class OpenGLShader : public Shader {
	public:
		virtual ~OpenGLShader();

		void Bind();
		void UnBind();

		void Init(const char* vertex_shader, const char* fragment_shader);

		/* Uniforms go here! */
		void Set1f(const std::string& name, float& value);

		uint32_t GetUniformLocation(const std::string& name);
	private:
		uint32_t shader_id;
	};
}

#endif // !OPENGL_SHADER_H
