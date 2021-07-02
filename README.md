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
	<li>Continue the GUI for the engine. Finish up the entity gui system and start adding more components. Sprite Component, Script Component, Lighting Component. Also add a renderer list for the scene that can be edited using frustum culling and other rendering fixes. This should improve performance.</li>
	<li>Add a scene serializer. Probably make our own using my file manager library.</li>
	<li>Create a directory viewer for our files and assets.</li>
	<li>Make a command console and logger where we can see output and commands from their game but also from the engine. (Anything that is being logged can be seen in the gui).</li>
	<li>Create a menu header for quick access to useful features (Stuff like file, edit, view).</li>
</ul>
