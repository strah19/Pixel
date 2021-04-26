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
		inline std::vector<Mesh>& GetMeshes() { return meshes; }
		inline std::string GetPath() const { return path; }
		inline std::vector<MeshTexture>& GetTextures() { return textures_loaded; }
	private:
		void LoadModel(const std::string& file_path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* mat, aiTextureType type);

		std::string path;
		std::vector<MeshTexture> textures_loaded;
		std::vector<Mesh> meshes;
	};
}

#endif // !MODEL_H