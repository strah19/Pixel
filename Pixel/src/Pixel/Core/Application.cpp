#include "pixelpch.h"
#include "Application.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommands.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

namespace Pixel {
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));
		instance = this;
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();

		texture1 = Pixel::Texture::CreateTexture("awesomeface.png");
		texture2 = Pixel::Texture::CreateTexture("texture1.jpg");
		camera = PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));
	}

	Application::~Application() { }

	void Application::Run() {
		while (is_running) {
			Pixel::RendererCommand::Clear();
			Pixel::RendererCommand::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			camera.Update();
			Pixel::Renderer::BeginScene(camera.GetCamera());

			for (int i = 0; i < 50; i++) {
				for (int j = 0; j < 50; j++) {
					if ((i + j) % 2 == 0)
						Pixel::Renderer::DrawQuad({ i, j, -2.0f }, { 0.2, 0.2 }, { 0.5f, 1.0f, 0, 0.7f }, texture1);
					else
						Pixel::Renderer::DrawQuad({ i, j, -2.0f }, { 0.2, 0.2 }, { 0.5f, 0.0f, 0.55f, 0.7f }, texture2);

				}
			}
			
			Pixel::Renderer::EndScene();

			window->Update();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(OnResize));
		dispatcher.Dispatch<QuitEvent>(PIXEL_BIND_EVENT(OnClose));

		camera.OnEvent(event);
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