#include "pixelpch.h"
#include "Mesh.h"
#include "Renderer/RendererCommands.h"
#include <glad/glad.h>

namespace Pixel {
    Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshTexture>& textures) {
        this->vertices = vertices;
        this->textures = textures;
        this->indices = indices;
        vertex_buffer = VertexBuffer::CreateVertexBuffer((uint32_t)(vertices.size() * sizeof(MeshVertex)));
        vertex_buffer->SetData(&vertices[0], (uint32_t)(vertices.size() * sizeof(MeshVertex)));

        VertexBufferLayout layout;
        layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float)); //Position
        layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float)); //Normals
        layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float)); //Texture Coordinates
        vertex_array = VertexArray::CreateVertexArray();
        vertex_buffer->SetLayout(layout);
        index_buffer = IndexBuffer::CreateIndexBuffer(&indices[0], indices.size() * sizeof(uint32_t));
        vertex_array->SetIndexBufferSize(index_buffer->GetCount());
        vertex_array->AddVertexBuffer(vertex_buffer);
    }

    void Mesh::Draw(std::shared_ptr<Shader>& shader) {
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        for (uint32_t i = 0; i < textures.size(); i++) {
            textures[i].texture->Bind(i);
            std::string number;
            std::string name = textures[i].texture_type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            float id = (float)i;
            shader->Set1f(name + number, id);
        }

        vertex_array->Bind();
        index_buffer->Bind();
        vertex_buffer->Bind();
        Pixel::RendererCommand::DrawVertexArray(vertex_array);
        vertex_array->UnBind();
        index_buffer->UnBind();
        vertex_buffer->UnBind();
    }
}