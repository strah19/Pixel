#include "pixelpch.h"
#include "ResourceManagers.h"

namespace Pixel {
	static ShaderSSBOS shader_storage_buffers;

	void SSBOManager::QueueForNewFrame() {
		for (auto& ssbo : shader_storage_buffers) {
			ssbo.second.complete = false;
		}
	}

	void SSBOManager::AddSSBOToManager(const std::string& name, std::shared_ptr<ShaderStorageBuffer>& ssbo) {
		bool add_to_storage = true;
		for (auto& in_storage_ssbo : shader_storage_buffers) {
			if (in_storage_ssbo.first == name) {
				add_to_storage = false;
				break;
			}
		}

		if (add_to_storage)
			shader_storage_buffers[name] = { ssbo, false };
	}

	ShaderSSBOS& SSBOManager::GetSSBOS() {
		return shader_storage_buffers; 
	}

	std::shared_ptr<ShaderStorageBuffer>* SSBOManager::SearchSSBOStorage(const std::string& shader_storage_name) {
		auto it = shader_storage_buffers.find(shader_storage_name);
		if (it != shader_storage_buffers.end()) {
			return &it->second.ssbo;
		}

		return nullptr;
	}

	void ShaderInfo::Init(const std::string& shader) {
		this->shader = Shader::CreateShader();
		this->shader->Init(shader);
	}

	bool ShaderInfo::AddSSBOReference(const std::string& shader_storage_name) {
		std::shared_ptr<ShaderStorageBuffer>* ssbo = SSBOManager::SearchSSBOStorage(shader_storage_name);
		if (ssbo) {
			shader_storage_refs.push_back(ssbo);
			return true;
		}

		return false;
	}

	SSBOData* ShaderInfo::GetBufferPointer(const std::string& name) {
		auto it = SSBOManager::GetSSBOS().find(name);
		if (it != SSBOManager::GetSSBOS().end()) {
			return &it->second;
		}

		return nullptr;
	}

	void ShaderInfo::UpdateSSBO(SSBOData* ssbo, uint32_t instance, void* data, uint32_t data_size) {
		if(!ssbo->complete)
			ssbo->ssbo->SetData(data, data_size, instance * data_size);
	}

	void ShaderInfo::SSBOUploadFinised(SSBOData* data) {
		data->complete = true;
	}
}