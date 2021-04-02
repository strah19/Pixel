#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <memory>
#include <glm.hpp>

namespace Pixel {
	struct ShaderSources {
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
		virtual void SetVec3f(const std::string& name, const glm::vec3& vec3) = 0;
		virtual void SetIntArray(const std::string& name, int* array) = 0;

		virtual uint32_t GetId() const = 0;
		virtual uint32_t GetUniformLocation(const std::string& name) = 0;
	private:
	};

	void ProgramSet1f(uint32_t id, const std::string& name, float& value);
	void ProgramSetMat4f(uint32_t id, const std::string& name, const glm::mat4& mat4);
	void ProgramSetVec3f(uint32_t id, const std::string& name, const glm::vec3& vec3);
	void ProgramSetIntArray(uint32_t id, const std::string& name, int* array);
	uint32_t ProgramGetUniformLocation(uint32_t id, const std::string& name);
	std::vector<std::string> GetUniformNames(uint32_t id);
}

#endif // !SHADER_H
