#include <iostream>
#include <Pixel.h>

using namespace Pixel;

class Sandbox : public Pixel::Application {
public:
	Sandbox() {
		Pixel::RendererCommand::Init();
		Pixel::Renderer::Init();
		Pixel::Renderer::InitDefaultShader();

		camera = PerspectiveCameraController(glm::vec2(1280.0f, 720.0f));

		texture1 = Texture::CreateTexture("texture1.jpg");
		texture2 = Texture::CreateTexture("awesomeface.png");;

		circle_shader = Shader::CreateShader();
		circle_shader->Init("shaders/colored_circle_shader.glsl");
		Renderer::InitRendererShader(circle_shader.get());
	}

	void OnUpdate(float delta) {
		Pixel::RendererCommand::Clear();
		Pixel::RendererCommand::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		camera.Update();
		Pixel::Renderer::BeginScene(camera.GetCamera());

		Pixel::Renderer::DrawQuad({ 2, 0, 0 }, { 1, 1 }, { 0.0, 1.0, 0.0, 1.0 });
		Pixel::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 0.0, 1.0, 0.0, 1.0 });
		Pixel::Renderer::DrawQuad({ 3, 0, 0 }, { 1, 1 }, { 0.0, 1.0, 0.0, 1.0 });

		Pixel::Renderer::EndScene();
	}

	void UserDefEvent(Pixel::Event& event) {
		Pixel::EventDispatcher dispatcher(&event);
		camera.OnEvent(event);
	}
private:
	std::shared_ptr<Texture> texture1;
	std::shared_ptr<Texture> texture2;
	PerspectiveCameraController camera;
	std::shared_ptr<Shader> circle_shader;
};

Pixel::Application* Pixel::CreateApplication()
{
	return new Sandbox;
}