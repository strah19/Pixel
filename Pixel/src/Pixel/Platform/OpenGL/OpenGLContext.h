#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "Renderer/RendererContext.h"
#include <GLFW/glfw3.h>

namespace Pixel {
	class OpenGLContext : public RendererContext {
	public:
		OpenGLContext(GLFWwindow* window);
		virtual ~OpenGLContext() = default;

		bool Init();
		void SwapBuffers();
		void SetVSync(int interval);
	private:
		GLFWwindow* window;
	};
}

#endif // !OPENGL_CONTEXT_H
