#include "pixelpch.h"
#include "Application.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommands.h"

#include "Renderer/Buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Models/Model.h"

//#include "ImGui/imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

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
		test_model.Init("obj/backpack/backpack.obj");
		/*
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 450");
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		*/
	}

	Application::~Application() { }

	void Application::Run() {
		while (is_running) {
			Pixel::RendererCommand::Clear();
			Pixel::RendererCommand::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

			camera.Update();
			
			Pixel::Renderer::BeginScene(camera.GetCamera());
			Pixel::Renderer::EndScene();

			model_shader->Bind();
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