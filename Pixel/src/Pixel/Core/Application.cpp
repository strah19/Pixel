#include "pixelpch.h"
#include "Application.h"
#include <glm/glm.hpp>

namespace Pixel {
	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));
	}

	void Application::Run() {
		while (is_running) {
			window->Update();
			std::cout << window->GetWidth() << std::endl;
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(OnResize));
		dispatcher.Dispatch<QuitEvent>(PIXEL_BIND_EVENT(OnClose));
	}

	bool Application::OnClose(const QuitEvent& event) {
		is_running = false;
		return true;
	}

	bool Application::OnResize(const ResizeEvent& event) {
		return true;
	}
}