#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Camera/Camera.h"
#include "Materials/Material.h"
#include "Models/Model.h"
#include "Core/ResourceManagers.h"
#include "Renderer/CubeMap.h"

namespace Pixel {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		glm::vec3 normals = glm::vec3(0, 0, 0);
		float material_id;
	};

	constexpr size_t MAX_QUAD_COUNT = 100000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * QUAD_VERTEX_COUNT;
	constexpr size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	constexpr size_t CUBE_FACES = 6;
	constexpr size_t MAX_TEXTURE_SLOTS = 32;
	constexpr size_t MAX_DRAW_COMMANDS = 1000;
	constexpr size_t MAX_INSTANCE_COUNT = 10000;
	constexpr size_t MAX_MATERIAL_COUNT = 64;
	constexpr size_t MAX_LIGHT_COUNT = 64;
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

	enum RenderFlags {
		SkyBoxFlag = 0x01, None = 0x02
	};

	class Renderer {
	public:
		static void Init();
		static void Destroy();

		static void SetShaderToDefualt();
		static void InitRendererShader(Shader* shader);
		static void SetShader(std::shared_ptr<Shader>* shader);
		static void SetMaterialId(uint32_t material_id);

		static uint32_t GetShaderId();

		static void BeginScene(Camera& camera, RenderFlags flags = RenderFlags::None);
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
		
		static void GoToNextDrawCommand();
		static void MakeCommand();
	private:
		static void StartBatch();
		static void Render();

		static float CalculateTextureIndex(std::shared_ptr<Texture>& texture);
		static void CalculateSquareIndices();
	};

	glm::vec3 CalculateVertexNormals(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
}

#endif // !RENDERER_H
