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
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name);
	private:
		std::string path;
		std::vector<Mesh> meshes;
		std::vector<MeshTexture> textures_loaded;
	};
}

#endif // !MODEL_H