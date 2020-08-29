#include <iostream>
#include <Pixel.h>

class Sandbox : public Pixel::Application {

};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}