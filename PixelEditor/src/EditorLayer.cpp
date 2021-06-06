#include "EditorLayer.h"
#include "Core/Logger.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Application.h"

namespace Pixel {
	EditorLayer::EditorLayer(std::shared_ptr<Pixel::FrameBuffer>* framebuffer) : Layer("Editor") {
		this->framebuffer = framebuffer;
		editor_camera = PerspectiveCameraController(glm::vec2(Pixel::Application::GetApp()->GetWindow()->GetWidth(), Pixel::Application::GetApp()->GetWindow()->GetHeight()));
	}

	void EditorLayer::OnAttach() {
	}

	void EditorLayer::OnDetach() {

	}

	void EditorLayer::OnUpdate(float delta) {
		if(activate_editor)
			editor_camera.Update();
	}

	void EditorLayer::UserDefEvent(Event& event) {
		if (activate_editor)
			editor_camera.OnEvent(event);
	}

	void EditorLayer::UpdateGui() {
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
			activate_editor = ImGui::IsWindowFocused();

			ImGui::BeginChild("GameRender");
			ImVec2 wsize = ImGui::GetWindowSize();
			uint32_t tex = framebuffer->get()->GetColorAttachment();
			ImGui::Image((void*)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		ImGui::End();



		ImGui::Begin("Log");
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();
	}
}