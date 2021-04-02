#include "pixelpch.h"
#include "Application.h"
#include "Renderer/RendererCommands.h"

#include "ImGui/ImGuiLayer.h"
#include "Core/Logger.h"

#include <GLFW/glfw3.h>

namespace Pixel {
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));
		instance = this;

		ImGuiLayer::Init();
	}

	Application::~Application() { 
		ImGuiLayer::Destroy();
		OnDestroy();
	}

	float Application::GetTick() {
		return (float) glfwGetTime();
	}

	void Application::Run() {
		while (is_running) {
			float time_since_start = (float) glfwGetTime();
			delta = time_since_start - old_time_since_start;
			old_time_since_start = time_since_start;

			OnUpdate(delta);

			window->Update();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(OnResize));
		dispatcher.Dispatch<QuitEvent>(PIXEL_BIND_EVENT(OnClose));
	}

	bool Application::OnClose(const QuitEvent& event) {
		is_running = false;
		return true;
	}

	bool Application::OnResize(const ResizeEvent& event) {
		Pixel::RendererCommand::SetViewport(0, 0, event.width, event.height);
		return true;
	}
}