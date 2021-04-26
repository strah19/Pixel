#include <iostream>
#include <Pixel.h>

glm::vec2 LIGHT_UV_COORDS[] = { { -1.0f, 1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f } };

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
		Pixel::InstanceRenderer::Init();

		camera = Pixel::PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Pixel::TextureManager::GetTextureManager()->CreateTexture("container_diffuse.png");
		framebuf = Pixel::FrameBuffer::Create(1280, 720);
		model.Init("obj/mars/planet.obj");
	}

	~Sandbox() {
		Pixel::Renderer::Destroy();
		Pixel::InstanceRenderer::Destroy();
	}

	void OnUpdate(float delta) {
		Pixel::SSBOManager::GetSSBOManager()->QueueForNewFrame();

		framebuf->Bind();
		Pixel::RendererCommand::Clear();
		Pixel::RendererCommand::SetClearColor(0.4f, 0.5f, 0.55f, 1.0f);

		Pixel::ImGuiLayer::Begin();

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", NULL, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();

		ImGui::Begin("Display");
		{
			bool focused = ImGui::IsWindowFocused();
			if (focused)
				camera.Update();

			ImGui::BeginChild("GameRender");
			ImVec2 wsize = ImGui::GetWindowSize();
			uint32_t tex = framebuf->GetColorAttachment();
			ImGui::Image((void*)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::End();

		ImGui::Begin("Scene Hierarchy");
		ImGui::End();

		ImGui::Begin("Log");
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();
		/*
		Pixel::Renderer::BeginScene(camera.GetCamera());

		Pixel::Renderer::DrawCube({ 0, 0, 0 }, { 1, 1, 1 }, { 0.9, 0.7, 0.4, 1.0 });
		Pixel::Renderer::MakeCommand();
		Pixel::Renderer::DrawQuad({ 1, 0, 0 }, { 1, 1 }, *texture1, { 0.5, 0.7, 0.8, 1.0 });
		Pixel::Renderer::DrawCube({ 2, 0, 0 }, { 1, 1, 1 }, { 0.9, 0.7, 0.4, 1.0 });
		Pixel::Renderer::DrawCube({ 3, 0, 0 }, { 1, 1, 1 }, { 0.9, 0.7, 0.4, 1.0 });
		Pixel::Renderer::DrawCube({ 4, 0, 0 }, { 1, 1, 1 }, { 0.9, 0.7, 0.4, 1.0 });

		Pixel::Renderer::MakeCommand();
		

		Pixel::Renderer::EndScene();
		*/

		Pixel::InstanceRenderer::BeginScene(camera.GetCamera());
		for (int i = 0; i < 100; i++) {
			Pixel::InstanceRenderer::DrawModel(&model, { cos(i) * 80 + 20, 0, sin(i) * 80 + 20 }, { 1, 1, 1 }, { 1, 1, 0, 1 });

		}

		Pixel::InstanceRenderer::MakeCommand();

		Pixel::InstanceRenderer::EndScene();
		framebuf->UnBind();

		Pixel::ImGuiLayer::End();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
	glm::vec4 light = { 1, 1, 1, 1 };
	glm::vec3 light_source = { 0.0f, 3.0f, 0.0f };
	std::shared_ptr<Pixel::Texture>* texture1;
	Pixel::PerspectiveCameraController camera;
	std::shared_ptr<Pixel::FrameBuffer> framebuf;
	Pixel::Model model;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}