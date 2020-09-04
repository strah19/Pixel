#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

#include "Platform/Window.h"
#include "Events/Events.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/RendererCommands.h"
#include "Renderer/Renderer2D.h"

int main(int argc, char** argv);

namespace Pixel {
	class Application {
	public:
		Application(const std::string& name = "Pixel Game Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		void Run();
		virtual void UserDefEvent(Event& event) { }
	private:
		bool is_running;
		std::unique_ptr<Window> window;

		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		std::shared_ptr<VertexArray> vertex;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<RendererCommands> commands;
		std::shared_ptr<Renderer2D> renderer;

		unsigned int texture1;
	};

	Application* CreateApplication();
}

#endif // !APPLICATION_H
