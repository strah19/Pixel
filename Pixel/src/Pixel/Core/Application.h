#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

#include "Platform/Window.h"
#include "Events/Events.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Camera/OrthoCamera.h"
#include "Camera/OrthoCameraController.h"
#include "Camera/PerspectiveCameraController.h"

int main(int argc, char** argv);

namespace Pixel {
	class Application {
	public:
		Application(const std::string& name = "Pixel Game Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		void Run();
		virtual void UserDefEvent(Event& event) { }
		
		static Application* GetApp() { return instance; }
		Window* GetWindow() { return window.get(); }
	private:
		bool is_running;
		std::unique_ptr<Window> window;

		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		std::shared_ptr<Texture> texture1;
		std::shared_ptr<Texture> texture2;
		PerspectiveCameraController camera;

		std::shared_ptr<Shader> cube_shader;

		static Application* instance; 
	};

	Application* CreateApplication();
}

#endif // !APPLICATION_H
