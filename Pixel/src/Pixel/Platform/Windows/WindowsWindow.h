#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "Platform/Window.h"
#include <GLFW/glfw3.h>

namespace Pixel {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProperties& properties);
		WindowsWindow();

		virtual ~WindowsWindow();

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		void* GetNativeWindow() const;
		void Update();
	private:
		GLFWwindow* native_window;
		WindowProperties properties;

		void Construct();
		bool Initialize();
		void Destroy();
	};
}

#endif // !WINDOWS_WINDOW_H
