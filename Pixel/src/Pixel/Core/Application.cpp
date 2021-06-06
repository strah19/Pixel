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
		assert(!instance);
			
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));
		instance = this;

		imgui_layer = new ImGuiLayer();
		PushLayer(imgui_layer);
	}

	Application::~Application() { 
		OnDestroy();
	}

	float Application::GetTick() {
		return (float) glfwGetTime();
	}

	void Application::Run() {
		while (is_running) {
			float time_since_start = GetTick();
			delta = time_since_start - old_time_since_start;
			old_time_since_start = time_since_start;

			for (Layer* layer : layers)
				layer->OnUpdate(delta);

			imgui_layer->Begin();
			for (Layer* layer : layers)
				layer->UpdateGui();
			imgui_layer->End();

			OnUpdate(delta);


			window->Update();
		}
	}

	void Application::PushLayer(Layer* layer) {
		layers.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		layers.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		for (auto it = layers.rbegin(); it != layers.rend(); ++it)
			(*it)->UserDefEvent(event);

		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(OnResize));
		dispatcher.Dispatch<QuitEvent>(PIXEL_BIND_EVENT(OnClose));
	}

	void Application::OnClose(const QuitEvent& event) {
		is_running = false;
	}

	void Application::OnResize(const ResizeEvent& event) {
		Pixel::RendererCommand::SetViewport(0, 0, event.width, event.height);
	}
}