#include "pixelpch.h"
#include "Application.h"

namespace Pixel {
	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
	}

	void Application::Run() {
		while (is_running) {
			is_running = window->Update();
		}
	}
}