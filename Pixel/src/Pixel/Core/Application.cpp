#include "pixelpch.h"
#include "Application.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommands.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

float vertices[] = {
 -0.8f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
-0.8f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,0.0f,

 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
 0.8f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,1.0f,
0.8f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f,
0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

namespace Pixel {
	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));

		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();

		texture1 = Pixel::Texture::CreateTexture("awesomeface.png");
	}

	void Application::Run() {
		while (is_running) {
			Pixel::RendererCommand::Clear();
			Pixel::RendererCommand::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			Pixel::Renderer::BeginScene();
			Pixel::Renderer::DrawQuad({ 0, 0 }, { 1, 1.2 }, { 255, 255, 0, 255 }, texture1);
			Pixel::Renderer::DrawQuad({ -0.5, 0 }, { 1, 1.2 }, { 0, 255, 0, 255 }, texture1);
			Pixel::Renderer::DrawQuad({ 0.5, 0 }, { 1, 1.2 }, { 255, 0, 255, 255 }, texture1);
			Pixel::Renderer::EndScene();

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
		Pixel::Renderer::Destroy();
		return true;
	}

	bool Application::OnResize(const ResizeEvent& event) {
		Pixel::RendererCommand::SetViewport(0, 0, event.width, event.height);
		return true;
	}
}