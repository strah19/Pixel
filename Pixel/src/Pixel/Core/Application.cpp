#include "pixelpch.h"
#include "Application.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"

"out vec3 outColor;\n"
"out vec2 TexCoord;\n"

"uniform float offset;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);\n"
"   outColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"
"in vec2 TexCoord;\n"

"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"  FragColor = texture(ourTexture, TexCoord) * vec4(outColor, 1.0);\n"
"}\0";

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
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		shader = Shader::CreateShader();
		shader->Init(vertexShaderSource, fragmentShaderSource);

		std::shared_ptr<VertexBuffer> buffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices));
		vertex = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		
		buffer->SetLayout(layout);

		vertex->SetIndexBuffer(IndexBuffer::CreateIndexBuffer(indices, sizeof(indices)));
		vertex->AddVertexBuffer(buffer);

		renderer = Pixel::Renderer2D::CreateRenderer();
		commands = Pixel::RendererCommands::CreateRendererCommands();

		texture = Pixel::Texture::CreateTexture("wall.jpg");
	}

	void Application::Run() {
		while (is_running) {
			commands->Clear();
			commands->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			float offset = 0.2f;
			shader->Set1f("offset", offset);

			texture->Bind();
			renderer->Submit(vertex, shader);

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