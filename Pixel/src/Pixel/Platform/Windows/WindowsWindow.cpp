#include "pixelpch.h"
#include "WindowsWindow.h"

namespace Pixel {
	static uint32_t glfw_window_count = 0;

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: properties(properties) {
		Construct();
	}

	WindowsWindow::WindowsWindow() {
		Construct();
	}

	WindowsWindow::~WindowsWindow(){
		Destroy();
	}

	uint32_t WindowsWindow::GetWidth() const {
		return properties.width;
		
	}

	uint32_t WindowsWindow::GetHeight() const {
		return properties.height;
	}

	void* WindowsWindow::GetNativeWindow() const {
		return native_window;
	}

	void WindowsWindow::Update() {
		glfwSwapBuffers(native_window);
		glfwPollEvents();
	}

	void WindowsWindow::Construct() {
		if (!Initialize())
			Destroy();
	}

	bool WindowsWindow::Initialize() {
		if (!glfwInit())
			return false;

		native_window = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), NULL, NULL);
		glfw_window_count++;

		return (native_window != nullptr);
	}

	void WindowsWindow::Destroy() {
		--glfw_window_count;
		glfwDestroyWindow(native_window);
		if (glfw_window_count == 0)
			glfwTerminate();
	}
}