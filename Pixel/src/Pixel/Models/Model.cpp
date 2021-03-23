#include "pixelpch.h"
#include "Model.h"
#include "Renderer/RendererCommands.h"
#include <glad/glad.h>

namespace Pixel {
    void Model::Init(const char* file_path) {
        path = file_path;
        LoadModel(file_path);

        uint32_t vertices_size = 0;
        uint32_t indices_size = 0;
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        for (size_t i = 0; i < meshes.size(); i++) {
            Mesh* past_mesh = nullptr;
            if (i > 0)
                past_mesh = &meshes[i - 1];
            vertices_size += meshes[i].vertices.size();
            indices_size += meshes[i].indices.size();

            vertices.insert(vertices.end(), meshes[i].vertices.begin(), meshes[i].vertices.end());
            if(past_mesh == nullptr)
                indices.insert(indices.end(), meshes[i].indices.begin(), meshes[i].indices.end());
            else {
                meshes[i].indices.front() = past_mesh->indices.back() + 1;
                for (int j = 1; j < meshes[i].indices.size(); j++) {
                    meshes[i].indices[j] = meshes[i].indices[j - 1] + 1;
                }
                indices.insert(indices.end(), meshes[i].indices.begin(), meshes[i].indices.end());
            }
        }

        vertex_buffer = VertexBuffer::CreateVertexBuffer((uint32_t)(vertices_size * sizeof(MeshVertex)));
        vertex_buffer->SetData(&vertices[0], (uint32_t)(vertices.size() * sizeof(MeshVertex)));

        VertexBufferLayout layout;
        layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float)); //Position
        layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float)); //Normals
        layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float)); //Texture Coordinates
        vertex_array = VertexArray::CreateVertexArray();
        vertex_buffer->SetLayout(layout);
        index_buffer = IndexBuffer::CreateIndexBuffer(&indices[0], indices_size * sizeof(uint32_t));
        vertex_array->SetIndexBufferSize(index_buffer->GetCount());
        vertex_array->AddVertexBuffer(vertex_buffer);
    }

    void Model::Draw(std::shared_ptr<Shader>& shader, Camera& camera) {
        shader->Bind();
        shader->SetMat4f("proj_view", glm::mat4(camera.GetProjection() * camera.GetView()));
        
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        for (uint32_t i = 0; i < textures_loaded.size(); i++) {
            textures_loaded[i].texture->Bind(i);
            std::string number;
            std::string name = textures_loaded[i].texture_type;
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
    }

    void Model::LoadModel(const std::string& file_path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }

        path = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene) {
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<MeshTexture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            MeshVertex vertex;

            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.tex_coords = vec;
            }
            else
                vertex.tex_coords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material,
                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material,
                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        
        return Mesh(vertices, indices, textures);
    }

    std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name) {
        std::vector<MeshTexture> textures;
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (uint32_t j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                MeshTexture texture;
                std::string filename = std::string(str.C_Str());
                filename = path + '/' + filename;
                texture.texture = Texture::CreateTexture(filename.c_str());
                texture.texture_type = type_name;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
}