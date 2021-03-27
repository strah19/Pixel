# Pixel

```c++
#include <iostream>
#include <Pixel.h>

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
		Pixel::Renderer::InitDefaultShader();
	}

	void OnUpdate(float delta) {
		Pixel::RendererCommand::Clear();
		Pixel::RendererCommand::SetClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
    
		Pixel::Renderer::BeginScene(camera.GetCamera());

		Pixel::Renderer::EndScene();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
  glm::vec3 background_color = { 0.4f, 0.5f, 0.55f };
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}

```
