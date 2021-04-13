#ifndef MESH_H
#define MESH_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Pixel {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		glm::vec3 normals = glm::vec3(0, 0, 0);
		float material_id;
	};

	struct MeshVertex {
		glm::vec3 position;
		glm::vec2 texture_coordinates;
		glm::vec3 normals = glm::vec3(0, 0, 0);
		float instance_id;
	};

	struct MeshTexture {
		std::shared_ptr<Texture> texture;
		std::string path;
	};

	struct Mesh {
		std::vector<MeshVertex> vertices;
		std::vector<uint32_t> indices;

		Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	};
}

#endif // !MESH_H