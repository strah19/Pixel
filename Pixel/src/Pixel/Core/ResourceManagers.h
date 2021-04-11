#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Renderer/Buffers.h"
#include "Renderer/Shader.h"
#include <vector>

namespace Pixel {
	struct SSBOData {
		std::shared_ptr<ShaderStorageBuffer> ssbo;
		bool complete;
	};

	using ShaderSSBOS = std::unordered_map<std::string, SSBOData>;

	class SSBOManager {
	public:
		static void QueueForNewFrame();
		static void AddSSBOToManager(const std::string& name, std::shared_ptr<ShaderStorageBuffer>& ssbo);
		static ShaderSSBOS& GetSSBOS();
		static std::shared_ptr<ShaderStorageBuffer>* SearchSSBOStorage(const std::string& shader_storage_name);
	};

	class ShaderInfo {
	public:
		void Init(const std::string& shader);
		bool AddSSBOReference(const std::string& shader_storage_name);
		std::shared_ptr<Shader>* GetShader() { return &shader; }
		SSBOData* GetBufferPointer(const std::string& name);

		void UpdateSSBO(SSBOData* ssbo, uint32_t instance, void* data, uint32_t data_size);
		void SSBOUploadFinised(SSBOData* ssbo);
	private:
		std::shared_ptr<Shader> shader;
		std::vector<std::shared_ptr<ShaderStorageBuffer>*> shader_storage_refs;
	};
}

#endif // !RESOURCE_MANAGER_H
