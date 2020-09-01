#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "Platform/Window.h"
#include <GLFW/glfw3.h>
#include "Renderer/RendererContext.h"

namespace Pixel {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProperties& properties);
		WindowsWindow();

		virtual ~WindowsWindow();

		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;

		void SetEventCallback(const EventCallbackFn& callback) override { data.event_call_back = callback; }
		void* GetNativeWindow() const override;
		void Update() override;
	private:
		GLFWwindow* native_window;
		std::unique_ptr<RendererContext> context;

		struct WindowData {
			WindowProperties properties;
			EventCallbackFn event_call_back;
		};

		WindowData data;

		void Construct();
		bool Initialize();
		void Destroy();
	};
}

#endif // !WINDOWS_WINDOW_H
