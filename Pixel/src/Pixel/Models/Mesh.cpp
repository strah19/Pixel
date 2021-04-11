#include "pixelpch.h"
#include "Mesh.h"

namespace Pixel {
    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshTexture>& textures) {
        this->vertices = vertices;
        this->textures = textures;
        this->indices = indices;
    }
}