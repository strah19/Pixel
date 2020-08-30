#include "pixelpch.h"
#include "Window.h"
#include "Core/PlatformDetection.h"

#ifdef PIXEL_WINDOW_PLATFORM
#include "Platform/Windows/WindowsWindow.h"
#endif // PIXEL_WINDOW_PLATFORM


namespace Pixel {
	std::unique_ptr<Window> Window::CreateWindow(const WindowProperties& properties) {
#ifdef PIXEL_WINDOW_PLATFORM
		return std::make_unique<WindowsWindow>();
#endif // PIXEL_WINDOW_PLATFORM
		return nullptr;
	}
}