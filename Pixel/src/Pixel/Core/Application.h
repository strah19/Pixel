#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

#include "Platform/Window.h"
#include "Events/Events.h"
#include "Events/WindowEvents.h"

int main(int argc, char** argv);

namespace Pixel {
	class Application {
	public:
		Application(const std::string& name = "Pixel Game Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& event);
	private:
		bool is_running;
		std::unique_ptr<Window> window;

		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
	};

	Application* CreateApplication();
}

#endif // !APPLICATION_H
