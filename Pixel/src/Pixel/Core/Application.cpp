#include "pixelpch.h"
#include "Application.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommands.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Models/Model.h"

namespace Pixel {
	Application* Application::instance = nullptr;
	static Model test_model;

	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));
		instance = this;
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();

		camera = PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Texture::CreateTexture("texture1.jpg");
		texture2 = Texture::CreateTexture("awesomeface.png");

		model_shader = Shader::CreateShader();
		model_shader->Init("shaders/model_shader.glsl");
		test_model.Init("obj/mars/planet.obj");
	}

	Application::~Application() { }

	void Application::Run() {
		while (is_running) {
			Pixel::RendererCommand::Clear();
			Pixel::RendererCommand::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			camera.Update();
			
			Pixel::Renderer::BeginScene(camera.GetCamera());
			Pixel::Renderer::DrawQuad({ 0.5, 0.5 }, { 1, 1 }, { 1.0, 1.0, 0.0, 1.0 });	
			Pixel::Renderer::EndScene();

			test_model.Draw(model_shader, camera.GetCamera());

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