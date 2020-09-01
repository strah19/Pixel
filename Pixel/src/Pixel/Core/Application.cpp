#include "pixelpch.h"
#include "Application.h"

#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"

namespace Pixel {
	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		window = Window::CreateWindow({ name, width, height });
		window->SetEventCallback(PIXEL_BIND_EVENT(OnEvent));

		uint32_t indices[]{
			0, 1, 3,
			1, 2, 3
		};

		float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<IndexBuffer> index = IndexBuffer::CreateIndexBuffer(indices, sizeof(indices));
		std::shared_ptr<VertexBuffer> buffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices));
		std::shared_ptr<VertexArray> vertex = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		
		vertex->SetVertexAttributes(layout);
	}

	void Application::Run() {
		while (is_running) {
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
		return true;
	}
}