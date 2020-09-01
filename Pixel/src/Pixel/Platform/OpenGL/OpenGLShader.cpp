#include "pixelpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Pixel {
	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(shader_id);
	}

	void OpenGLShader::Bind() {
		glUseProgram(shader_id);
	}

	void OpenGLShader::UnBind() {
		glUseProgram(0);
	}

	void OpenGLShader::Init(const char* vertex_shader, const char* fragment_shader){
		int32_t is_success;
		char error[512];

		uint32_t ver_shader;
		ver_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ver_shader, 1, &vertex_shader, NULL);
		glCompileShader(ver_shader);
		glGetShaderiv(ver_shader, GL_COMPILE_STATUS, &is_success);

		if (!is_success) {
			glGetShaderInfoLog(ver_shader, 512, NULL, error);
			std::cout << error << std::endl;
		}

		uint32_t frag_shader;
		frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, &fragment_shader, NULL);
		glCompileShader(frag_shader);
		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &is_success);

		if (!is_success) {
			glGetShaderInfoLog(frag_shader, 512, NULL, error);
			std::cout << error << std::endl;
		}

		shader_id = glCreateProgram();
		glAttachShader(shader_id, ver_shader);
		glAttachShader(shader_id, frag_shader);

		glLinkProgram(shader_id);
		glDeleteShader(ver_shader);
		glDeleteShader(frag_shader);

		glGetProgramiv(shader_id, GL_LINK_STATUS, &is_success);
		if (!is_success) {
			glGetProgramInfoLog(shader_id, 512, NULL, error);
			std::cout << error << std::endl;
		}
	}

	void OpenGLShader::Set1f(const std::string& name, float& value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& name) {
		return (glGetUniformLocation(shader_id, name.c_str()));
	}
}