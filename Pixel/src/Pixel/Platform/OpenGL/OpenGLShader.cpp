#include "pixelpch.h"
#include "OpenGLShader.h"
#include "Renderer/RendererAPI.h"
#include "Core/Logger.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace Pixel {
	static uint32_t current_shader_binded = 0;

	OpenGLShader::OpenGLShader(const std::string& file_path) {
		Init(file_path);
	}

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

	uint32_t OpenGLShader::CompileShader(const std::string& source, uint32_t type) {
		uint32_t id = glCreateShader(type);
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
			PIXEL_LOG_ERROR("SHADER::%s::FAILED::%s", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderSources OpenGLShader::ParseShader(const std::string& file_path) {
		std::ifstream stream(file_path);

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		ShaderType type = ShaderType::NONE;
		std::string line;
		std::stringstream ss[2];

		if (!stream.is_open()) {
			PIXEL_LOG_ERROR("SHADER_LOAD_FAILED::%s", file_path.c_str());
			return { ss[0].str(), ss[1].str() };
		}
		else
			PIXEL_LOG("LOADED_SHADER::%s", file_path.c_str());

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

	uint32_t OpenGLShader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader) {
		uint32_t program = glCreateProgram();
		uint32_t vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
		uint32_t fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	void OpenGLShader::Set1f(const std::string& name, float value) {
		ProgramSet1f(shader_id, name, value);
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

	void ProgramSet1f(uint32_t id, const std::string& name, float value) {
		glProgramUniform1f(id, ProgramGetUniformLocation(id, name), value);
	}

	void ProgramSetMat4f(uint32_t id, const std::string& name, const glm::mat4& mat4) {
		glProgramUniformMatrix4fv(id, ProgramGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void ProgramSetVec3f(uint32_t id, const std::string& name, const glm::vec3& vec3) {
		glProgramUniform3f(id, ProgramGetUniformLocation(id, name), vec3.x, vec3.y, vec3.z);
	}

	void ProgramSetIntArray(uint32_t id, const std::string& name, int* array) {
		glProgramUniform1iv(id, ProgramGetUniformLocation(id, name), sizeof(array) / sizeof(int), array);
	}

	uint32_t ProgramGetUniformLocation(uint32_t id, const std::string& name) {
		return (glGetUniformLocation(id, name.c_str()));
	}

	std::vector<std::string> GetUniformNames(uint32_t id) {
		GLint i;
		GLint count;

		GLint size;
		GLenum type;

		const GLsizei bufSize = 16;
		GLchar name[bufSize];
		GLsizei length;
		std::vector<std::string> names;

		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++) {
			glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);
			names.push_back(name);
		}

		return names;
	}
}