#ifndef RENDERER_H
#define RENDERER_H

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Pixel {
	class Renderer {
	public:
		static void BeginScene();	/* Will take in a camera object.*/
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray> vertex_array, std::shared_ptr<Shader> shader);		/* Will take in a shader and vertex buffer to draw. */
	};
}

#endif // !RENDERER_H
