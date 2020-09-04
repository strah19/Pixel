#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Pixel {
	class Renderer2D {
	public:
		virtual void BeginScene() = 0;	/* Will take in a camera object.*/
		virtual void EndScene() = 0;

		virtual void Submit(std::shared_ptr<VertexArray> vertex_array, std::shared_ptr<Shader> shader) = 0;		/* Will take in a shader and vertex buffer to draw. */

		static std::shared_ptr<Renderer2D> CreateRenderer();
	};
}

#endif // !RENDERER_H
