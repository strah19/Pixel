#ifndef MESH_H
#define MESH_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Pixel {
	struct ModelMeshVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	struct ModelMeshTexture {
		std::shared_ptr<Texture> texture;
		std::string texture_type;
		std::string path;
	};

	struct ModelMesh {
		std::vector<ModelMeshVertex> vertices;
		std::vector<ModelMeshTexture> textures;
		std::vector<uint32_t> indices;

		ModelMesh(std::vector<ModelMeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<ModelMeshTexture>& textures);
	};
}

#endif // !MESH_H