#ifndef MESH_H
#define MESH_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Pixel {
	struct MeshVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	struct MeshTexture {
		std::shared_ptr<Texture> texture;
		std::string texture_type;
		std::string path;
	};

	class Mesh {
	public:
		std::vector<MeshVertex> vertices;
		std::vector<MeshTexture> textures;
		std::vector<uint32_t> indices;

		void Draw(std::shared_ptr<Shader>& shader);
		Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshTexture>& textures);
	private:

	};
}

#endif // !MESH_H