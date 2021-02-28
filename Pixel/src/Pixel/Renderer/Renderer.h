#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Pixel {
	class Renderer {
	public:
		static void Init();
		static void Destroy();
		static void BeginScene();	/* Will take in a camera object.*/
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<Shader>& shader);		/* Will take in a shader and vertex buffer to draw. */

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture);
	};
}

#endif // !RENDERER_H
