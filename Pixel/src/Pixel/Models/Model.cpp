#include "pixelpch.h"
#include "Model.h"
#include "Renderer/RendererCommands.h"
#include "Core/Logger.h"
#include <filesystem>

namespace Pixel {
    void Model::Init(const char* file_path) {
        path = file_path;
        LoadModel(file_path);

        size_t found = std::string(file_path).find_last_of('/');
        std::string dir = std::string(file_path).substr(0, found);
    }

    void Model::LoadModel(const std::string& file_path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            PIXEL_LOG_ERROR("ERROR::ASSIMP::%s", import.GetErrorString());
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
            vertex.normals = vector;

            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texture_coordinates = vec;
            }
            else
                vertex.texture_coordinates = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            LoadMaterialTextures(material, aiTextureType_DIFFUSE);
            LoadMaterialTextures(material, aiTextureType_SPECULAR);
            LoadMaterialTextures(material, aiTextureType_NORMALS);
        }
        
        return Mesh(vertices, indices);
    }

    void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type) {
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (uint32_t j = 0; j < textures_loaded.size(); j++) {
                if (textures_loaded[j].path == str.C_Str()) {
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                MeshTexture texture;
                texture.path = str.C_Str();
                std::string filename = std::string(str.C_Str());
                filename = path + '/' + filename;
                texture.texture = Texture::CreateTexture(filename.c_str());
                textures_loaded.push_back(texture);
            }
        }
    }
}
