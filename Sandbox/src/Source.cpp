#include <iostream>
#include <Pixel.h>

glm::vec2 LIGHT_UV_COORDS[] = { { -1.0f, 1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f } };

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();

		camera = Pixel::PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Pixel::Texture::CreateTexture("container_diffuse.png");
		texture2 = Pixel::Texture::CreateTexture("container_specular.png");;

		light_shader = Pixel::Shader::CreateShader();
		light_shader->Init("shaders/light_texture.glsl");
		Pixel::Renderer::InitRendererShader(light_shader.get());

		two_d_light_shader = Pixel::Shader::CreateShader();
		two_d_light_shader->Init("shaders/uniform_block_test.glsl");
		Pixel::Renderer::InitRendererShader(two_d_light_shader.get());

		framebuf = Pixel::FrameBuffer::Create(1280, 720);

		s.Init(texture1);
		s.Divide({ 16, 16 });
	}

	void OnUpdate(float delta) {
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
		/*
		ImGui::SliderFloat("Shininess", &m.shininess, -10.0f, 10.0f); 
		ImGui::SliderFloat3("Specular", &m.light_source.specular.x, 0.0f, 20.0f);
		ImGui::SliderFloat3("Diffuse", &m.light_source.diffuse.x, 0.0f, 20.0f);
		ImGui::SliderFloat3("Ambient", &m.light_source.ambient.x, 0.0f, 20.0f);
		*/

		ImGui::End();

		ImGui::Begin("Scene Hierarchy");
		ImGui::End();

		ImGui::Begin("Log");
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();


		Pixel::Renderer::BeginScene(camera.GetCamera());
		//Pixel::Renderer::SetShader(&two_d_light_shader);
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if((i + j) % 2 == 0)
					Pixel::Renderer::DrawQuad({ i, j, 0 }, { 1, 1 }, light);
				else 
					Pixel::Renderer::DrawQuad({ i, j, 0 }, { 1, 1 }, { 1, 1, 0, 1 });
			}
		}

		
		/*
		Pixel::Renderer::SetShader(&light_shader);
		m.diffuse = &texture1;
		m.specular = &texture2;
		m.initialized = true;
		m.light_source.position = light_source;
		m.light_source.view_pos = camera.GetCamera().GetPosition();
		Pixel::Renderer::SetMaterial(&m);
		Pixel::Renderer::DrawCube({ 0, 0, 10 }, { 20, 1, 20 }, { 1, 0, 0, 1 });
		
		
		Pixel::Renderer::SetShader(&light_shader);
		m.ambient = { 0.0215,	0.1745,	0.0215 };
		m.diffuse = { 0.07568,	0.61424,	0.07568 };
		m.specular = { 0.633,	0.727811,	0.633 };
		m.shininess = 0.6;
		m.initialized = true;
		m.light_source.ambient = { 0.2f, 0.2f, 0.2f };
		m.light_source.diffuse = { 0.5f, 0.5f, 0.5f };
		m.light_source.specular = { 1.0f, 1.0f, 1.0f };
		m.light_source.position = light_source;
		m.light_source.view_pos = camera.GetCamera().GetPosition();

		Pixel::Renderer::SetMaterial(&m);
		Pixel::Renderer::DrawCube({ 0, 0, 0 }, { 1, 1, 1 }, { 1, 0, 0, 1 });
		Pixel::Renderer::DrawCube({ 3, 0, 0 }, { 1, 1, 1 }, { 1, 0, 0, 1 });

		
		Pixel::Renderer::SetShader(&light_shader);

		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "lightColor", glm::vec3(light));
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "lightPos", light_source);
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "viewPos", camera.GetCamera().GetPosition());
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "material.ambient", { 1.0f, 0.5f, 0.31f });
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "material.specular", { 0.5f, 0.5f, 0.5f });
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "material.diffuse", { 1.0f, 0.5f, 0.31f });

		float shin = 32;
		Pixel::ProgramSet1f(Pixel::Renderer::GetShaderId(), "material.shininess", shin);

		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "light.ambient", { 0.2f, 0.2f, 0.2f });
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "light.diffuse", { 0.5f, 0.5f, 0.5f }); 
		Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "light.specular", { 1.0f, 1.0f, 1.0f });

		Pixel::Renderer::DrawCube({ 0, 0, 0 }, { 10, 1, 10 }, texture1, { 1.0f, 0.5f, 0.31f, 1.0 });
		*/
		Pixel::Renderer::EndScene();
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
	std::shared_ptr<Pixel::Texture> texture1;
	std::shared_ptr<Pixel::Texture> texture2;
	Pixel::PerspectiveCameraController camera;
	std::shared_ptr<Pixel::Shader> light_shader;
	std::shared_ptr<Pixel::Shader> two_d_light_shader;
	std::shared_ptr<Pixel::FrameBuffer> framebuf;
	Pixel::Spritesheet s;
	Pixel::TextureMaterial m;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}