#include "pixelpch.h"
#include "WindowsWindow.h"
#include "glad/glad.h"

namespace Pixel {
	static uint32_t glfw_window_count = 0;

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: data({ properties }) {
		Construct();
	}

	WindowsWindow::WindowsWindow() {
		Construct();
	}

	WindowsWindow::~WindowsWindow(){
		Destroy();
	}

	uint32_t WindowsWindow::GetWidth() const {
		return data.properties.width;	
	}

	uint32_t WindowsWindow::GetHeight() const {
		return data.properties.height;
	}

	void* WindowsWindow::GetNativeWindow() const {
		return native_window;
	}

	bool WindowsWindow::Update() {
		if (!glfwWindowShouldClose(native_window)) {
			glfwSwapBuffers(native_window);
			glfwPollEvents();
			return true;
		}
		return false;
	}

	void WindowsWindow::Construct() {
		if (!Initialize())
			Destroy();
	}

	bool WindowsWindow::Initialize() {
		if (!glfwInit())
			return false;

		native_window = glfwCreateWindow(data.properties.width, data.properties.height, data.properties.title.c_str(), NULL, NULL);
		glfw_window_count++;

		glfwMakeContextCurrent(native_window);
		glfwSetWindowUserPointer(native_window, &data);
		int error = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		return (native_window != nullptr);
	}

	void WindowsWindow::Destroy() {
		--glfw_window_count;
		glfwDestroyWindow(native_window);
		if (glfw_window_count == 0)
			glfwTerminate();
	}
}