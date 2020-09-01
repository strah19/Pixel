#include "pixelpch.h"
#include "Application.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>

const char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"

"out vec3 outColor;\n"
"uniform float offset;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);\n"
"   outColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"

"void main()\n"
"{\n"
"  FragColor = vec4(outColor, 1.0);\n"
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
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);	//creates empty shader object with type
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	//gives shader source to shader object
		glCompileShader(vertexShader);	//compiles source code in shader object
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram = glCreateProgram();	//creates program which can attach shaders
		glAttachShader(shaderProgram, vertexShader);	//attaches shader object to rpogram
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);	//links all shader objects attached together
		glDeleteShader(vertexShader);	//deletes shader P.S- once shader linked to program the shader is no longer needed
		glDeleteShader(fragmentShader);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		}

		std::shared_ptr<VertexBuffer> buffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices));
		vertex = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		
		buffer->SetLayout(layout);

		vertex->SetIndexBuffer(IndexBuffer::CreateIndexBuffer(indices, sizeof(indices)));
		vertex->AddVertexBuffer(buffer);
	}

	void Application::Run() {
		while (is_running) {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			glUseProgram(shaderProgram);
			glBindVertexArray(vertex->GetId());
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