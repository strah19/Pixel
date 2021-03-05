#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"

namespace Pixel {
	class Renderer {
	public:
		~Renderer();
		static void Init();
		static void BeginScene(OrthoCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<Shader>& shader);		

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id);
	private:
		static void StartBatch();
		static void Render();
		static void NewBatch();
		static float CalculateTextureIndex(std::shared_ptr<Texture>& texture);
	};
}

#endif // !RENDERER_H
