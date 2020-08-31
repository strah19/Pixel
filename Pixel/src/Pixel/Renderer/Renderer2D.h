#ifndef RENDERER_H
#define RENDERER_H

namespace Pixel {
	class Renderer2D {
	public:
		virtual void BeginScene() = 0;	/* Will take in a camera object.*/
		virtual void EndScene() = 0;

		virtual void Submit() = 0;		/* Will take in a shader and vertex buffer to draw. */
	};
}

#endif // !RENDERER_H
