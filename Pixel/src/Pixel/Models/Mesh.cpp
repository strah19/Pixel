#include "pixelpch.h"
#include "Mesh.h"

namespace Pixel {
    Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices) {
        this->vertices = vertices;
        this->indices = indices;
    }
}