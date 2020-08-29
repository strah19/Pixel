#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

int main(int argc, char** argv);

namespace Pixel {
	class Application {
	public:
		Application(const std::string& name = "Pixel game Engine");
		virtual ~Application() = default;

	private:
	};

	Application* CreateApplication();
}

#endif // !APPLICATION_H
