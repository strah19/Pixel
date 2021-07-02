# Pixel

```c++
#include <Pixel.h>

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
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

<h1>To Do</h1>

<ul style=" list-style-type: none;">
	<li>Refactor ALL renderering code (get rid of smart pointers and do some bentchmarking), finish up the instance renderer for models.<li>
	<li>Test the material system with different lighting effects.<li>
</ul>
