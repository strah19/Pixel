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

<ul>
	<li>Refactor ALL renderering code (get rid of smart pointers and do some benchmarking), finish up the instance renderer for models. Refer to the Ember renderer for the batch renderer. </li>
	<li>Test the material system with different lighting effects.</li>
	<li>Try to fix the cubemap bug.</li>
	<li>Implement a simple lighting system that is derived from Entities.</li>
	<li>Try out an animation system.</li>
	<li>(FUTURE) Make my own model loader and math library</li>
	<li></li>
</ul>
