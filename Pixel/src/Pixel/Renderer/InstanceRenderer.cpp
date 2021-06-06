#include "pixelpch.h"
#include "InstanceRenderer.h"
#include "Renderer.h"
#include <glad/glad.h>
#include "RendererCommands.h"
#include "Core/Utility/Timer.h"

namespace Pixel {
	struct InstanceRendererData {
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;
		std::shared_ptr<IndirectDrawBuffer> indirect_draw_buffer;

		ShaderInfo* current_shader;
		ShaderInfo default_shader;

		uint32_t index_offset = 0;

		uint32_t texture_slot_index = 0;
		std::shared_ptr<Texture>* textures[MAX_TEXTURE_SLOTS];
		glm::mat4 proj_view = glm::mat4(1.0f);

		uint32_t num_of_vertices_in_batch = 0;

		DrawElementsCommand draw_commands[MAX_DRAW_COMMANDS];
		uint32_t base_vert = 0;
		uint32_t draw_count = 0;
		uint32_t current_draw_command_vertex_size = 0;

		InstanceVertex* vertices_base = nullptr;
		InstanceVertex* vertices_ptr = nullptr;

		glm::vec4* color_base = nullptr;
		glm::vec4* color_ptr = nullptr;

		glm::mat4* model_base = nullptr;
		glm::mat4* model_ptr = nullptr;

		uint32_t* index_base = nullptr;
		uint32_t* index_ptr = nullptr;

		std::vector<Model*> in_buffer_models;
		uint32_t instance_count = 0;
	};

	static InstanceRendererData renderer_data;

	void InstanceRenderer::Init() {
		renderer_data.vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(Vertex) * MAX_VERTEX_COUNT);
		renderer_data.vertex_array = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(1, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));

		renderer_data.vertex_buffer->SetLayout(layout);

		renderer_data.vertices_base = new InstanceVertex[MAX_VERTEX_COUNT];
		renderer_data.index_base = new uint32_t[MAX_INDEX_COUNT];
		renderer_data.color_base = new glm::vec4[MAX_INSTANCE_COUNT];
		renderer_data.model_base = new glm::mat4[MAX_INSTANCE_COUNT];

		renderer_data.index_buffer = IndexBuffer::CreateIndexBuffer(MAX_INDEX_COUNT * sizeof(uint32_t));
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());

		renderer_data.indirect_draw_buffer = IndirectDrawBuffer::CreateIndirectDrawBuffer(sizeof(renderer_data.draw_commands));

		renderer_data.default_shader.Init("shaders/instance_shader.glsl");
		Renderer::InitRendererShader(renderer_data.default_shader.GetShader()->get());

		renderer_data.default_shader.AddSSBOReference("GlobalMatrices", sizeof(glm::mat4));
	}

	void InstanceRenderer::Destroy() {
		delete[] renderer_data.vertices_base;
		delete[] renderer_data.index_base;
		delete[] renderer_data.color_base;
		delete[] renderer_data.model_base;
	}

	void InstanceRenderer::BeginScene(Camera& camera) {
		Pixel::SSBOManager::GetSSBOManager()->QueueForNewFrame();
		renderer_data.proj_view = camera.GetProjection() * camera.GetView();
		renderer_data.current_shader = &renderer_data.default_shader;
		StartBatch();
	}

	void InstanceRenderer::EndScene() {
		Render();
	}

	void InstanceRenderer::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color) {
		renderer_data.instance_count++;

		glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		*renderer_data.model_ptr = model_mat;
		renderer_data.model_ptr++;

		*renderer_data.color_ptr = color;
		renderer_data.color_ptr++;

		for (auto& m : renderer_data.in_buffer_models) {
			if (m == model) {
				return;
			}
		}
		float tex_id = -1.0f;
		if(!model->GetTextures().empty())
			tex_id = CalculateTextureIndex(model->GetTextures()[0].texture);

		for (size_t mesh = 0; mesh < model->GetMeshes().size(); mesh++) {
			Mesh* past_mesh = nullptr;
			if (mesh > 0)
				past_mesh = &model->GetMeshes()[mesh - 1];
			else if (mesh == 0 && !renderer_data.in_buffer_models.empty())
				past_mesh = &renderer_data.in_buffer_models.back()->GetMeshes().back();
			for (size_t vertex = 0; vertex < model->GetMeshes()[mesh].vertices.size(); vertex++) {
				renderer_data.vertices_ptr->position = model->GetMeshes()[mesh].vertices[vertex].position;
				renderer_data.vertices_ptr->texture_coordinates = model->GetMeshes()[mesh].vertices[vertex].texture_coordinates;

				renderer_data.vertices_ptr->texture_id = tex_id;
				renderer_data.vertices_ptr++;
			}
			renderer_data.num_of_vertices_in_batch += (uint32_t) model->GetMeshes()[mesh].vertices.size() + 1;

			if (past_mesh != nullptr) {
				model->GetMeshes()[mesh].indices.front() = past_mesh->indices.back() + 1;
				for (size_t index = 1; index < model->GetMeshes()[mesh].indices.size(); index++) 
					model->GetMeshes()[mesh].indices[index] = model->GetMeshes()[mesh].indices[index - 1] + 1;
			}

			for (size_t index = 0; index < model->GetMeshes()[mesh].indices.size(); index++) {
				*renderer_data.index_ptr = model->GetMeshes()[mesh].indices[index];
				renderer_data.index_ptr++;
			}
			renderer_data.current_draw_command_vertex_size += (uint32_t) model->GetMeshes()[mesh].indices.size();
		}

		renderer_data.in_buffer_models.push_back(model);
	}

	void InstanceRenderer::StartBatch() {
		renderer_data.texture_slot_index = 0;
		renderer_data.num_of_vertices_in_batch = 0;
		renderer_data.index_offset = 0;

		renderer_data.base_vert = 0;
		renderer_data.draw_count = 0;
		renderer_data.current_draw_command_vertex_size = 0;

		renderer_data.vertices_ptr = renderer_data.vertices_base;
		renderer_data.index_ptr = renderer_data.index_base;
		renderer_data.model_ptr = renderer_data.model_base;
		renderer_data.color_ptr = renderer_data.color_base;

		renderer_data.in_buffer_models.clear();
		renderer_data.instance_count = 0;
	}

	void InstanceRenderer::Render() {
		renderer_data.vertex_array->Bind();
		renderer_data.index_buffer->Bind();
		renderer_data.vertex_buffer->Bind();

		renderer_data.indirect_draw_buffer->Bind();
		renderer_data.indirect_draw_buffer->SetData(renderer_data.draw_commands, sizeof(renderer_data.draw_commands), 0);

		std::shared_ptr<Shader>* shader = renderer_data.current_shader->GetShader();
		shader->get()->Bind();

		if (shader == renderer_data.default_shader.GetShader()) {
			SSBOData* ssbo = renderer_data.current_shader->GetBufferPointer("GlobalMatrices");
			ssbo->ssbo->Bind();
			renderer_data.current_shader->UpdateSSBO(ssbo, 0, (void*)&renderer_data.proj_view, sizeof(glm::mat4));
			renderer_data.current_shader->SSBOUploadFinised(ssbo);
			ssbo->ssbo->BindToBindPoint();
		}

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
			renderer_data.textures[i]->get()->Bind(i);

		uint32_t vertex_buf_size = (uint32_t)((uint8_t*)renderer_data.vertices_ptr - (uint8_t*)renderer_data.vertices_base);
		uint32_t index_buf_size = (uint32_t)((uint8_t*)renderer_data.index_ptr - (uint8_t*)renderer_data.index_base);
		uint32_t color_buf_size = (uint32_t)((uint8_t*)renderer_data.color_ptr - (uint8_t*)renderer_data.color_base);
		uint32_t matrix_buf_size = (uint32_t)((uint8_t*)renderer_data.model_ptr - (uint8_t*)renderer_data.model_base);

		uint32_t offsets[] = {
			0, 
			sizeof(glm::vec3), 
			sizeof(glm::vec3) + sizeof(glm::vec2), 
			vertex_buf_size, 
			vertex_buf_size + color_buf_size, 
			vertex_buf_size + color_buf_size + 16, 
			vertex_buf_size + color_buf_size + 32, 
			vertex_buf_size + color_buf_size + 48
		};
		static uint32_t stride[] = { sizeof(InstanceVertex), sizeof(InstanceVertex), sizeof(InstanceVertex), 16, 64, 64, 64, 64 };
		renderer_data.vertex_array->SetArrayForInstancing(renderer_data.vertex_buffer, offsets, stride);

		renderer_data.vertex_buffer->SetData((void*)renderer_data.vertices_base, vertex_buf_size, 0);
		renderer_data.vertex_buffer->SetData((void*)renderer_data.color_base, color_buf_size, vertex_buf_size);
		renderer_data.vertex_buffer->SetData((void*)renderer_data.model_base, matrix_buf_size, vertex_buf_size + color_buf_size);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		renderer_data.index_buffer->SetData(renderer_data.index_base, index_buf_size);

		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());

		renderer_data.indirect_draw_buffer->Bind();
		renderer_data.indirect_draw_buffer->SetData(renderer_data.draw_commands, sizeof(renderer_data.draw_commands), 0);
		RendererCommand::DrawMultiIndirect(nullptr, renderer_data.draw_count + 1, 0);
	}

	void InstanceRenderer::NewBatch() {
		Render();
		StartBatch();
	}

	void InstanceRenderer::GoToNextDrawCommand() {
		renderer_data.draw_count++;
		renderer_data.base_vert += renderer_data.current_draw_command_vertex_size;
		renderer_data.current_draw_command_vertex_size = 0;
		renderer_data.instance_count = 0;
	}

	void InstanceRenderer::MakeCommand() {
		renderer_data.draw_commands[renderer_data.draw_count].vertex_count = renderer_data.current_draw_command_vertex_size;
		renderer_data.draw_commands[renderer_data.draw_count].instance_count = renderer_data.instance_count;
		renderer_data.draw_commands[renderer_data.draw_count].first_index = 0;
		renderer_data.draw_commands[renderer_data.draw_count].base_vertex = renderer_data.base_vert;
		renderer_data.draw_commands[renderer_data.draw_count].base_instance = renderer_data.draw_count;
	}

	float InstanceRenderer::CalculateTextureIndex(std::shared_ptr<Texture>& texture) {
		float texture_id = -1.0f;

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
			if (*renderer_data.textures[i] == texture)
				texture_id = (float)i;

		if (texture_id == -1.0f) {
			renderer_data.textures[renderer_data.texture_slot_index] = &texture;
			texture_id = (float)renderer_data.texture_slot_index;
			renderer_data.texture_slot_index++;

			if (renderer_data.texture_slot_index == MAX_TEXTURE_SLOTS)
				NewBatch();
		}

		return texture_id;
	}
}