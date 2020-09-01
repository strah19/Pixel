#include "pixelpch.h"
#include "Application.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <stb_image.h>

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

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int w, h, nrChannels;
		unsigned char* data = stbi_load("wall.jpg", &w, &h, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture" << std::endl;		
		stbi_image_free(data);

		shader = Shader::Create();
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
	}

	void Application::Run() {
		while (is_running) {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			shader->Bind();

			float offset = 0.2f;
			shader->Set1f("offset", offset);

			glBindVertexArray(vertex->GetId());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex->GetIndexBuffers()->GetId());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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