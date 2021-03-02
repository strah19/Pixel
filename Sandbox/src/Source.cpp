#include <iostream>
#include <Pixel.h>

class Sandbox : public Pixel::Application {
public:
	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		dispatcher.Dispatch<Pixel::KeyboardEvents>(PIXEL_BIND_EVENT(KeyPress));
	}
private:
	bool KeyPress(const Pixel::KeyboardEvents& event) {
	
		return true;
	}
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}