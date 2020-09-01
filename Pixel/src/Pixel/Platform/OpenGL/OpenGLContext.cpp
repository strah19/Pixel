#include "pixelpch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"

namespace Pixel {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: window(window) {
	}

	bool OpenGLContext::Init() {
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return false;
		return true;
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(window);
	}

	void OpenGLContext::SetVSync(int interval) {
		glfwSwapInterval(interval);
	}
}