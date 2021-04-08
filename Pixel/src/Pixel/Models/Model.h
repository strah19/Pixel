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
		std::vector<ModelMesh> meshes;
	private:
		void LoadModel(const std::string& file_path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		ModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<ModelMeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name);

		std::string path;
		std::vector<ModelMeshTexture> textures_loaded;
	};
}

#endif // !MODEL_H