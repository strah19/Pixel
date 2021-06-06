#include <Pixel.h>

#include "EditorLayer.h"
#include "Panels/SceneHierarchy.h"

namespace Pixel {
	class Editor : public Pixel::Application {
	public:
		Editor() {
			editor = new EditorLayer(&framebuffer);
			PushLayer(editor);
			PushLayer(new SceneHierarchy());

			Pixel::RendererCommand::Init();
			Pixel::Renderer::Init();
			Pixel::InstanceRenderer::Init();

			material_shader.Init("shaders/color_material_shaders/material_point_light_shader.glsl");
			material_shader.AddSSBOReference("Materials", (sizeof(Pixel::Material) * MAX_MATERIAL_COUNT) + sizeof(Pixel::LightSourcePoint) + sizeof(glm::vec4));
			framebuffer = Pixel::FrameBuffer::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight());
		}

		~Editor() {
			Pixel::Renderer::Destroy();
			Pixel::InstanceRenderer::Destroy();
		}

		void OnUpdate(float delta) {
			framebuffer->Bind();

			Pixel::RendererCommand::Clear();
			Pixel::RendererCommand::SetClearColor(0.4f, 0.5f, 0.55f, 1.0f);
			//camera.Update();
			Pixel::Renderer::BeginScene(editor->GetEditorCamera().GetCamera(), Pixel::RenderFlags::None);

			Pixel::Renderer::SetShaderToDefualt();
			Pixel::Renderer::DrawQuad({ 0, 0, 1 }, { 1, 1 }, { 1, 0, 0, 1 });
			Pixel::Renderer::MakeCommand();
			Pixel::Renderer::NewBatch();

			Pixel::Renderer::SetShader(material_shader.GetShader());
			Pixel::Renderer::SetMaterialId(0);

			Pixel::LightSourcePoint light = Pixel::LightSourcePoint({ 0.2, 0.2, 0.2 }, { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0 }, { 0, 0, 1 }, 0.6f, 0.009f, 0.0032f);
			Pixel::Material material = Pixel::Material({ 0.0215,	0.1745,	0.0215 }, { 0.07568,	0.61424,	0.07568 }, { 0.633,	0.727811,	0.633 }, 32);

			Pixel::SSBOData* ssbo = material_shader.GetBufferPointer("Materials");

			ssbo->ssbo->Bind();
			ssbo->ssbo->SetData((void*)&light, sizeof(Pixel::LightSourcePoint), 0);
			ssbo->ssbo->SetData((void*)&editor->GetEditorCamera().GetCamera().GetPosition(), sizeof(glm::vec3), sizeof(Pixel::LightSourcePoint));
			ssbo->ssbo->SetData((void*)&material, sizeof(Pixel::Material), sizeof(Pixel::LightSourcePoint) + sizeof(glm::vec4));

			ssbo->ssbo->BindToBindPoint();
			material_shader.SSBOUploadFinised(ssbo);

			Pixel::Renderer::DrawQuad({ -5, 5, 0 }, { 30, 30 }, { 0, 1, 0, 1 });

			Pixel::Renderer::MakeCommand();

			Pixel::Renderer::EndScene();

			/*
			Pixel::InstanceRenderer::BeginScene(camera.GetCamera());
			Pixel::InstanceRenderer::DrawModel(&model2, { 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 0, 1 });
			Pixel::InstanceRenderer::MakeCommand();
			Pixel::InstanceRenderer::GoToNextDrawCommand();
			Pixel::InstanceRenderer::DrawModel(&model, { 7, 0, 0 }, { 1, 1, 1 }, { 1, 1, 0, 1 });
			Pixel::InstanceRenderer::MakeCommand();
			Pixel::InstanceRenderer::EndScene();
			*/
			framebuffer->UnBind();
			//ImGuiLayer::Update(&framebuffer);
		}

		void UpdateGui() {

		}

		void UserDefEvent(Pixel::Event& event) {
			Pixel::EventDispatcher dispatcher(&event);
		}
	private:
		EditorLayer* editor;
		std::shared_ptr<Pixel::FrameBuffer> framebuffer;

		Pixel::ShaderInfo material_shader;
	};

	Pixel::Application* Pixel::CreateApplication()
	{
		return new Editor;
	}
}
