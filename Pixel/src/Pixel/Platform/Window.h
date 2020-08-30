#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>

namespace Pixel {
	struct WindowProperties {
		std::string title;
		uint32_t width, height;

		WindowProperties()
			: title("Pixel Game Engine"), width(1280), height(720) { }
		WindowProperties(const std::string& title, uint32_t width, uint32_t height)
			: title(title), width(width), height(height) { }
	};

	class Window {
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void Update() = 0;

		static std::unique_ptr<Window> CreateWindow(const WindowProperties& properties);
	};
}

#endif // !WINDOW_H
