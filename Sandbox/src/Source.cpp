#include <iostream>
#include <Pixel.h>

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
		Pixel::Renderer::InitDefaultShader();

		camera = Pixel::PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Pixel::Texture::CreateTexture("texture1.jpg");
		texture2 = Pixel::Texture::CreateTexture("awesomeface.png");;

		light_shader = Pixel::Shader::CreateShader();
		light_shader->Init("shaders/light_source.glsl");
		Pixel::Renderer::InitRendererShader(light_shader.get());
	}

	void OnUpdate(float delta) {
		Pixel::RendererCommand::Clear();
		Pixel::RendererCommand::SetClearColor(0.4f, 0.5f, 0.55f, 1.0f);
		camera.Update();
		Pixel::Renderer::BeginScene(camera.GetCamera());
		Pixel::Renderer::DrawCube(light_source, { 1, 1, 1 }, light);

		Pixel::Renderer::SetShader(&light_shader);
		Pixel::Renderer::AddUniformsToShader([&](Pixel::Shader* shader) {
			shader->SetVec3f("lightColor", glm::vec3(light));
			shader->SetVec3f("lightPos", light_source);
			shader->SetVec3f("viewPos", camera.GetCamera().GetPosition());

		});
		Pixel::Renderer::DrawCube({ 0, 0, 0 }, { 10, 1, 10 }, { 1.0f, 0.5f, 0.31f, 1.0 });

		
		Pixel::Renderer::EndScene();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
	glm::vec4 light = { 0.0f, 5.0f, 1.0f, 1.0f };
	glm::vec3 light_source = { -5.0f, 1.0f, -5.0f };
	std::shared_ptr<Pixel::Texture> texture1;
	std::shared_ptr<Pixel::Texture> texture2;
	Pixel::PerspectiveCameraController camera;
	std::shared_ptr<Pixel::Shader> light_shader;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}