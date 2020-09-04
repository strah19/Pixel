#ifndef OPENGL_RENDERER2D_H
#define OPENGL_RENDERER2D_H

#include "Renderer/Renderer2D.h"

namespace Pixel {
	class OpeGLRenderer2D : public Renderer2D{
	public:
		void BeginScene();	/* Will take in a camera object.*/
		void EndScene();

		void Submit(std::shared_ptr<VertexArray> vertex_array, std::shared_ptr<Shader> shader);		/* Will take in a shader and vertex buffer to draw. */
	private:

	};
}

#endif // !OPENGL_RENDERER2D_H
