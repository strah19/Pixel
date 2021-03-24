#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Camera/Camera.h"

namespace Pixel {
	class Model {
	public:
		void Init(const char* file_path);
		void Draw(std::shared_ptr<Shader>& shader, Camera& camera);
	private:
		void LoadModel(const std::string& file_path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		ModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<ModelMeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name);
	private:
		std::string path;
		std::vector<ModelMesh> meshes;
		std::vector<ModelMeshTexture> textures_loaded;

		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;
	};
}

#endif // !MODEL_H