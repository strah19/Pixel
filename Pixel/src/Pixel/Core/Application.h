#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

#include "Platform/Window.h"
#include "Events/Events.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"
#include "Renderer/FrameBuffer.h"
#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Pixel {
	class Application {
	public:
		Application(const std::string& name = "Pixel Game Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUpdate(float delta) { }
		virtual void OnDestroy() { }

		static float GetTick();
		static Application* GetApp() { return instance; }
		Window* GetWindow() { return window.get(); }
	private:
		bool is_running;
		std::unique_ptr<Window> window;
		LayerStack layers;
		ImGuiLayer* imgui_layer;

		float delta = 0.0f;
		float old_time_since_start = 0.0f;

		void OnClose(const QuitEvent& event);
		void OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		static Application* instance; 
	};

	Application* CreateApplication();
}

#endif // !APPLICATION_H
