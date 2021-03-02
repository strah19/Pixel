#include "pixelpch.h"
#include "WindowsWindow.h"
#include "glad/glad.h"

#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"

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

	void WindowsWindow::Update() {
		context->SwapBuffers();
		glfwPollEvents();
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

		glfwSetWindowUserPointer(native_window, &data);

		context = RendererContext::CreateRendererContext(this);
		if (!context->Init())
			return false;
		context->SetVSync(1);

		glfwSetWindowSizeCallback(native_window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.properties.width = width;
			data.properties.height = height;

			ResizeEvent event(width, height);
			data.event_call_back(event);
		});

		glfwSetWindowCloseCallback(native_window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			QuitEvent event;
			data.event_call_back(event);
		});

		glfwSetCursorPosCallback(native_window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MousePositionEvent event((float)x, (float)y);
			data.event_call_back(event);
		});

		glfwSetScrollCallback(native_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseWheelEvent event((double) xoffset, (double) yoffset);
			data.event_call_back(event);
		});

		glfwSetKeyCallback(native_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyboardEvents event(key, scancode, action);
			data.event_call_back(event);
		});

		glfwSetMouseButtonCallback(native_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseButtonEvents event(action, button);
			data.event_call_back(event);
		});

		return (native_window != nullptr);
	}

	void WindowsWindow::Destroy() {
		--glfw_window_count;
		glfwDestroyWindow(native_window);
		if (glfw_window_count == 0)
			glfwTerminate();
	}
}