#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Camera/Camera.h"
#include "Lighting/Material.h"

namespace Pixel {
	constexpr size_t MAX_QUAD_COUNT = 1000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * QUAD_VERTEX_COUNT;
	constexpr size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	constexpr size_t MAX_TEXTURE_SLOTS = 32;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr glm::vec4 QUAD_POSITIONS[QUAD_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	constexpr size_t CUBE_VERTEX_COUNT = 24;
	constexpr glm::vec3 CUBE_POSITIONS[CUBE_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f },{ 0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, -1.0f }, { 0.5f,  0.5f, -1.0f }, { -0.5f,  0.5f, -1.0f },
		{ -0.5f, -0.5f, -1.0f }, { -0.5f, -0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, -1.0f },
		{ 0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f,  0.5f, 0.0f }, { 0.5f,  0.5f, -1.0f },
		{ -0.5f, 0.5f, -1.0f }, { 0.5f, 0.5f, -1.0f }, { 0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, -1.0f }, { 0.5f,  -0.5f, 0.0f }, { -0.5f,  -0.5f, 0.0f }
	};

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		glm::vec3 normals = glm::vec3(0, 0, 0);
	};

	struct RenderMesh {
		std::vector<Vertex> vertex_buffer_data;
		std::vector<uint32_t> indices;
		std::shared_ptr<Shader>* shader = nullptr;
		Material* material;
	};

	class Renderer {
	public:
		static void Init();

		static void SetShaderToDefualt();
		static void InitRendererShader(Shader* shader);
		static void SetShader(std::shared_ptr<Shader>* shader);
		static void SetMaterial(Material* material);

		static uint32_t GetShaderId();

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void NewBatch();

		static void Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<IndexBuffer>& index_buffer, std::shared_ptr<Shader>& shader);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, std::shared_ptr<Texture>& texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]);

		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]);
		static void AddMesh(RenderMesh& mesh);
	private:
		static void StartBatch();
		static void Render();
		static void InitDefaultShader();

		static float CalculateTextureIndex(std::shared_ptr<Texture>& texture);
		static RenderMesh* FindMesh();
	};

	glm::vec3 CalculateVertexNormals(const glm::vec3 triangle[]);
	void CalculateVertexNormalsAsRects(RenderMesh& mesh);
}

#endif // !RENDERER_H
