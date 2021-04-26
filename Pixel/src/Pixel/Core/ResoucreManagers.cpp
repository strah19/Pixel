#include "pixelpch.h"
#include "ResourceManagers.h"

namespace Pixel {
	template <class T>
	void ResourceManager<T>::AddToManager(const std::string& name, const T& data) {
		loaded_resources[name] = data;
	}

	template <class T>
	std::unordered_map<std::string, T>& ResourceManager<T>::GetLoadedData() {
		return loaded_resources;
	}

	template <class T>
	T* ResourceManager<T>::SearchStorage(const std::string& name) {
		auto it = loaded_resources.find(name);
		if (it != loaded_resources.end()) {
			return &it->second;
		}

		return nullptr;
	}

	SSBOManager* SSBOManager::manager = nullptr;
	void SSBOManager::QueueForNewFrame() {
		auto& ssbo_list = GetLoadedData();
		for (auto& ssbo : ssbo_list) {
			ssbo.second.complete = false;
		}
	}

	SSBOManager* SSBOManager::GetSSBOManager() {
		if (!manager) 
			manager = new SSBOManager();	

		return manager;
	}

	SSBOManager::~SSBOManager() {
		delete manager;
	}

	ShaderManager* ShaderManager::manager = nullptr;
	std::shared_ptr<Shader>* ShaderManager::CreateShader(const std::string& file_path) {
		std::shared_ptr<Shader>* shader = SearchStorage(file_path);
		if (!shader)
			AddToManager(file_path, Shader::CreateShader(file_path));

		return &GetLoadedData()[file_path];
	}

	ShaderManager* ShaderManager::GetShaderManager() {
		if (!manager) 
			manager = new ShaderManager;
		
		return manager;
	}

	ShaderManager::~ShaderManager() {
		delete manager;
	}

	TextureManager* TextureManager::manager = nullptr;
	std::shared_ptr<Texture>* TextureManager::CreateTexture(const std::string& file_path) {
		std::shared_ptr<Texture>* texture = SearchStorage(file_path);
		if (!texture)
			AddToManager(file_path, Texture::CreateTexture(file_path.c_str()));

		return &GetLoadedData()[file_path];
	}

	TextureManager* TextureManager::GetTextureManager() {
		if (!manager)
			manager = new TextureManager;

		return manager;
	}

	TextureManager::~TextureManager() {
		delete manager;
	}

	ModelManager* ModelManager::manager = nullptr;
	std::shared_ptr<Model>* ModelManager::CreateModel(const std::string& file_path) {
		std::shared_ptr<Model>* model = SearchStorage(file_path);
		if (!model) {
			AddToManager(file_path, std::make_shared<Model>());
			GetLoadedData()[file_path]->Init(file_path.c_str());
		}

		return &GetLoadedData()[file_path];
	}

	ModelManager* ModelManager::GetModelManager() {
		if (!manager)
			manager = new ModelManager;

		return manager;
	}

	ModelManager::~ModelManager() {
		delete manager;
	}

	void ShaderInfo::Init(const std::string& shader) {
		this->shader = ShaderManager::GetShaderManager()->CreateShader(shader);
	}

	bool ShaderInfo::AddSSBOReference(const std::string& shader_storage_name, uint32_t size) {
		SSBOData* ssbo_data = SSBOManager::GetSSBOManager()->SearchStorage(shader_storage_name);
		std::shared_ptr<ShaderStorageBuffer>* ssbo = &ssbo_data->ssbo;
		if (!ssbo)
			SSBOManager::GetSSBOManager()->AddToManager(shader_storage_name, { ShaderStorageBuffer::CreateShaderStorageBuffer(size), false });

		for (auto& ssbo_ref : shader_storage_refs) 
			if (ssbo_ref == ssbo) 
				return false;

		shader_storage_refs.push_back(ssbo);
		return true;
	}

	SSBOData* ShaderInfo::GetBufferPointer(const std::string& name) {
		auto it = SSBOManager::GetSSBOManager()->GetLoadedData().find(name);
		if (it != SSBOManager::GetSSBOManager()->GetLoadedData().end()) {
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