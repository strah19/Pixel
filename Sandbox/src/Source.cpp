#include <iostream>
#include <Pixel.h>

glm::vec2 LIGHT_UV_COORDS[] = { { -1.0f, 1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f } };

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
		Pixel::Renderer::InitDefaultShader();

		camera = Pixel::PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Pixel::Texture::CreateTexture("texture1.jpg");
		texture2 = Pixel::Texture::CreateTexture("awesomeface.png");;

		light_shader = Pixel::Shader::CreateShader();
		light_shader->Init("shaders/light_source.glsl");
		Pixel::Renderer::InitRendererShader(light_shader.get());

		two_d_light_shader = Pixel::Shader::CreateShader();
		two_d_light_shader->Init("shaders/fake_lighting.glsl");
		Pixel::Renderer::InitRendererShader(two_d_light_shader.get());

		Pixel::InitializeLoggingSystem();
		Pixel::LogFormat format;
		format.Init("[{ts}]::{s}::{l}::{s}\n", "Pixel", "ERROR");
		Pixel::SetLogFormat(&format);
		Pixel::Log("%s", "Hello World");
		Pixel::Log("SUP%s", "World");

		framebuf = Pixel::FrameBuffer::Create(1280, 720);
	}

	void OnUpdate(float delta) {
		framebuf->Bind();
		Pixel::RendererCommand::Clear();
		Pixel::RendererCommand::SetClearColor(0.4f, 0.5f, 0.55f, 1.0f);

		camera.Update();
		Pixel::Renderer::BeginScene(camera.GetCamera());
		
		Pixel::Renderer::DrawQuad({ -1.0f, -1.0f, -0.5f }, { 5.0f, 5.0f }, { 1.0, 0.0, 0.4, 1.0 });
		Pixel::Renderer::SetShader(&two_d_light_shader);
		Pixel::Renderer::DrawQuad({ -1.0f, -1.0f, 0.0f }, { 20.0f, 20.0f }, LIGHT_UV_COORDS, { 0.7, 0.5, 0.0, 1.0 });

		//Pixel::Renderer::DrawCube(light_source, { 1, 1, 1 }, light);

		//Pixel::Renderer::SetShader(&light_shader);

		//Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "lightColor", glm::vec3(light));
		//Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "lightPos", light_source);
		//Pixel::ProgramSetVec3f(Pixel::Renderer::GetShaderId(), "viewPos", camera.GetCamera().GetPosition());

		//Pixel::Renderer::DrawCube({ 0, 0, 0 }, { 10, 1, 10 }, { 1.0f, 0.5f, 0.31f, 1.0 });

		Pixel::Renderer::EndScene();
		framebuf->UnBind();

		Pixel::ImGuiLayer::Begin();

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
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

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", NULL, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();

		ImGui::Begin("Display");

		{
			// Using a Child allow to fill all the space of the window.
			// It also alows customization
			ImGui::BeginChild("GameRender");
			// Get the size of the child (i.e. the whole draw size of the windows).
			ImVec2 wsize = ImGui::GetWindowSize();
			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			uint32_t tex = framebuf->GetColorAttachment();
			ImGui::Image((void*)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::End();

		ImGui::Begin("Scene Hierarchy");
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();

		Pixel::ImGuiLayer::End();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
	glm::vec4 light = { 0.9f, 0.7f, 0.7f, 1.0f };
	glm::vec3 light_source = { -5.0f, 1.0f, -5.0f };
	std::shared_ptr<Pixel::Texture> texture1;
	std::shared_ptr<Pixel::Texture> texture2;
	Pixel::PerspectiveCameraController camera;
	std::shared_ptr<Pixel::Shader> light_shader;
	std::shared_ptr<Pixel::Shader> two_d_light_shader;
	std::shared_ptr<Pixel::FrameBuffer> framebuf;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}