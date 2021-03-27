#include "pixelpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace Pixel {
	static uint32_t current_shader_binded = 0;

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(shader_id);
	}

	void OpenGLShader::Bind() {
		if (shader_id != current_shader_binded) {
			glUseProgram(shader_id);
			current_shader_binded = shader_id;
		}
	}

	void OpenGLShader::UnBind() {
		glUseProgram(0);
		current_shader_binded = 0;
	}

	void OpenGLShader::Init(const std::string& file_path) {
		ShaderSources source = ParseShader(file_path);
		shader_id = CreateShader(source.vertex, source.fragment);
	}

	uint32_t OpenGLShader::CompileShader(const std::string& source, uint32_t type)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::FAILED::" << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderSources OpenGLShader::ParseShader(const std::string& filePath)
	{
		std::ifstream stream(filePath);

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		ShaderType type = ShaderType::NONE;
		std::string line;
		std::stringstream ss[2];

		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else {
				ss[(int)type] << line << '\n';
			}
		}
		stream.close();
		return { ss[0].str(), ss[1].str() };
	}

	unsigned int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
		unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	void OpenGLShader::Set1f(const std::string& name, float& value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& name) {
		return (glGetUniformLocation(shader_id, name.c_str()));
	}

	void OpenGLShader::SetMat4f(const std::string& name, const glm::mat4& mat4) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void OpenGLShader::SetVec3f(const std::string& name, const glm::vec3& vec3) {
		glUniform3f(GetUniformLocation(name), vec3.x, vec3.y, vec3.z);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* array) {
		glUniform1iv(GetUniformLocation(name), sizeof(array) / sizeof(int), array);
	}
}