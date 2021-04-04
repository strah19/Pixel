#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Camera/Camera.h"
#include "Models/Mesh.h"

namespace Pixel {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		glm::vec3 normals = glm::vec3(0, 0, 0);
	};

	struct Material {
		glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };
		glm::vec3 diffuse = { 0.0f, 0.0f, 0.0f };
		glm::vec3 specular = { 0.0f, 0.0f, 0.0f };
		float shininess = 0.0f;

		Material(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, float shininess) 
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) { }

		Material() = default;
	};

	struct RenderMesh {
		std::vector<Vertex> vertex_buffer_data;
		std::vector<uint32_t> indices;
		std::shared_ptr<Shader>* shader = nullptr;
	};

	class Renderer {
	public:
		static void Init();

		static void InitDefaultShader();
		static void SetShaderToDefualt();
		static void InitRendererShader(Shader* shader);
		static void SetShader(std::shared_ptr<Shader>* shader);

		static uint32_t GetShaderId();

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<IndexBuffer>& index_buffer, std::shared_ptr<Shader>& shader);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]);
		static void AddMesh(RenderMesh& mesh);
	private:
		static void StartBatch();
		static void Render();
		static void NewBatch();

		static float CalculateTextureIndex(std::shared_ptr<Texture>& texture);
		static RenderMesh* FindMesh();
	};
}

#endif // !RENDERER_H
