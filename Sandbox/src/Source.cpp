#include <iostream>
#include <Pixel.h>
#include "gtc/type_ptr.hpp"

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
		model2.Init("obj/rock/rock.obj");
		std::vector<std::string> faces = 
		{
				"skybox/right.jpg",
				"skybox/left.jpg",
				"skybox/top.jpg",
				"skybox/bottom.jpg",
				"skybox/front.jpg",
				"skybox/back.jpg"
		};

		material_shader.Init("shaders/color_material_shaders/material_point_light_shader.glsl");
		material_shader.AddSSBOReference("Materials", (sizeof(Pixel::Material) * 64) + sizeof(Pixel::LightSourcePoint) + sizeof(glm::vec4));
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
		
		Pixel::Renderer::BeginScene(camera.GetCamera(), Pixel::RenderFlags::None);
		
		Pixel::Renderer::SetShaderToDefualt();
		Pixel::Renderer::DrawQuad({ 0, 0, 1 }, { 1, 1 }, { 1, 0, 0, 1 });
		Pixel::Renderer::MakeCommand();
		Pixel::Renderer::NewBatch();

		Pixel::Renderer::SetShader(material_shader.GetShader());
		Pixel::Renderer::SetMaterialId(0);

		Pixel::LightSourcePoint light = Pixel::LightSourcePoint({ 0.2, 0.2, 0.2 }, { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0 }, { 0, 0, 1 }, 0.6f, 0.009f, 0.0032f);
		Pixel::Material material = Pixel::Material({ 0.0215,	0.1745,	0.0215 }, { 0.07568,	0.61424,	0.07568 }, { 0.633,	0.727811,	0.633 }, 32);

		Pixel::SSBOData* ssbo = material_shader.GetBufferPointer("Materials");

		ssbo->ssbo->Bind();
		ssbo->ssbo->SetData((void*)&light, sizeof(Pixel::LightSourcePoint), 0);
		ssbo->ssbo->SetData((void*)&camera.GetCamera().GetPosition(), sizeof(glm::vec3), sizeof(Pixel::LightSourcePoint));
		ssbo->ssbo->SetData((void*)&material, sizeof(Pixel::Material), sizeof(Pixel::LightSourcePoint) + sizeof(glm::vec4));

		ssbo->ssbo->BindToBindPoint();
		material_shader.SSBOUploadFinised(ssbo);

		Pixel::Renderer::DrawQuad({ -5, 5, 0 }, { 20, 20 }, { 0, 1, 0, 1 });

		Pixel::Renderer::MakeCommand();		
		
		Pixel::Renderer::EndScene();

		/*
		Pixel::InstanceRenderer::BeginScene(camera.GetCamera());
		Pixel::InstanceRenderer::DrawModel(&model2, { 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 0, 1 });
		Pixel::InstanceRenderer::MakeCommand();
		Pixel::InstanceRenderer::GoToNextDrawCommand();
		Pixel::InstanceRenderer::DrawModel(&model, { 7, 0, 0 }, { 1, 1, 1 }, { 1, 1, 0, 1 });
		Pixel::InstanceRenderer::MakeCommand();
		Pixel::InstanceRenderer::EndScene();
		*/
		framebuf->UnBind();

		Pixel::ImGuiLayer::End();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
	glm::vec3 light_source = { 0.0f, 3.0f, 0.0f };
	std::shared_ptr<Pixel::Texture>* texture1;
	Pixel::PerspectiveCameraController camera;
	std::shared_ptr<Pixel::FrameBuffer> framebuf;
	Pixel::Model model;
	Pixel::Model model2;
	Pixel::ShaderInfo material_shader;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}