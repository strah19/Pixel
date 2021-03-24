#include "pixelpch.h"
#include "Mesh.h"

namespace Pixel {
    ModelMesh::ModelMesh(std::vector<ModelMeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<ModelMeshTexture>& textures) {
        this->vertices = vertices;
        this->textures = textures;
        this->indices = indices;
    }
}