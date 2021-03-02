#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Camera/OrthoCamera.h"

namespace Pixel {
	class Renderer {
	public:
		~Renderer();
		static void Init();
		static void BeginScene(OrthoCamera& camera);	
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<Shader>& shader);		/* Will take in a shader and vertex buffer to draw. */

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);

		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color, uint32_t texture_id);
		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color);
	};
}

#endif // !RENDERER_H
