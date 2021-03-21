#include "pixelpch.h"
#include "Renderer.h"
#include "Renderer/RendererCommands.h"

#include <glad/glad.h>
#include <array>
#include <gtc/matrix_transform.hpp>

namespace Pixel {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
	};

	constexpr size_t MAX_QUAD_COUNT = 1000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * QUAD_VERTEX_COUNT;
	constexpr size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	constexpr size_t MAX_TEXTURE_SLOTS = 32;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	struct RendererData {
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;
		std::shared_ptr<Shader> shader;

		Vertex* vertices_base = nullptr;
		Vertex* vertices_ptr = nullptr;

		uint32_t* index_base = nullptr;
		uint32_t* index_ptr = nullptr;
		uint32_t index_offset = 0;

		glm::vec4 quad_positions[4];
		uint32_t texture_slot_index = 0;
		std::shared_ptr<Texture>* textures[MAX_TEXTURE_SLOTS];
		glm::mat4 proj_view;

		int num_of_quads_in_batch = 0;
	};

	static RendererData renderer_data;

	void Renderer::Init() {
		renderer_data.vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(Vertex) * MAX_VERTEX_COUNT);
		renderer_data.vertex_array = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(1, false, VertexShaderType::Float));

		renderer_data.vertex_buffer->SetLayout(layout);

		renderer_data.vertices_base = new Vertex[MAX_VERTEX_COUNT];
		renderer_data.index_base = new uint32_t[MAX_INDEX_COUNT];
		renderer_data.index_buffer = IndexBuffer::CreateIndexBuffer(MAX_INDEX_COUNT * sizeof(uint32_t));
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());
		renderer_data.vertex_array->AddVertexBuffer(renderer_data.vertex_buffer);

		renderer_data.shader = Shader::CreateShader();
		renderer_data.shader->Init("shaders/shader.glsl");

		renderer_data.quad_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		renderer_data.shader->Bind();
		auto loc = renderer_data.shader->GetUniformLocation("ourTexture");
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
			sampler[i] = i;
		}
		glUniform1iv(loc, MAX_TEXTURE_SLOTS, sampler);

		renderer_data.vertex_array->UnBind();
		renderer_data.index_buffer->UnBind();
		renderer_data.vertex_buffer->UnBind();
		renderer_data.shader->UnBind();
	}

	void Renderer::BeginScene(Camera& camera) {
		renderer_data.proj_view = camera.GetProjection() * camera.GetView();
		StartBatch();
	}

	void Renderer::EndScene() {
		Render();
		renderer_data.shader->UnBind();
	}

	void Renderer::StartBatch() {
		renderer_data.vertices_ptr = renderer_data.vertices_base;
		renderer_data.shader->Bind();
		renderer_data.shader->SetMat4f("proj_view", renderer_data.proj_view);
		renderer_data.texture_slot_index = 0;
		renderer_data.num_of_quads_in_batch = 0;
		renderer_data.index_ptr = renderer_data.index_base;
		renderer_data.index_offset = 0;
	}

	void Renderer::Render() {
		uint32_t size = (uint32_t)((uint8_t*)renderer_data.vertices_ptr - (uint8_t*)renderer_data.vertices_base);

		if (size == 0) 
			return;		

		renderer_data.vertex_buffer->SetData(renderer_data.vertices_base, size);
		uint32_t index_size = (uint32_t)((uint8_t*)renderer_data.index_ptr - (uint8_t*)renderer_data.index_base);
		renderer_data.index_buffer->SetData(renderer_data.index_base, index_size);
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++) {
			renderer_data.textures[i]->get()->Bind(i);
		}

		renderer_data.vertex_array->Bind();
		renderer_data.index_buffer->Bind();
		renderer_data.vertex_buffer->Bind();
		RendererCommand::DrawVertexArray(renderer_data.vertex_array);
		renderer_data.vertex_array->UnBind();
		renderer_data.index_buffer->UnBind();
		renderer_data.vertex_buffer->UnBind();

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++) {
			renderer_data.textures[i]->get()->UnBind();
		}
	}

	void Renderer::NewBatch() {
		Render();
		StartBatch();
	}

	Renderer::~Renderer() {
		delete[] renderer_data.vertices_base;
		delete[] renderer_data.index_base;
	}

	void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<IndexBuffer>& index_buffer, std::shared_ptr<Shader>& shader) {
		shader->Bind();
		vertex_array->Bind();
		index_buffer->Bind();

		Pixel::RendererCommand::DrawVertexArray(vertex_array);
	}

	void Renderer::DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id) {
		*renderer_data.index_ptr = 0 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 1 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 2 + renderer_data.index_offset;
		renderer_data.index_ptr++;

		*renderer_data.index_ptr = 2 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 3 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 0 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		renderer_data.index_offset += 4;

		for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
			renderer_data.vertices_ptr->position = translation * renderer_data.quad_positions[i];
			renderer_data.vertices_ptr->color = color;
			renderer_data.vertices_ptr->texture_coordinates = TEX_COORDS[i];
			renderer_data.vertices_ptr->texture_id = texture_id;
			renderer_data.vertices_ptr++;
		}
		renderer_data.num_of_quads_in_batch++;
		if (renderer_data.num_of_quads_in_batch == MAX_QUAD_COUNT)
			NewBatch();
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, -1.0f);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, CalculateTextureIndex(texture));
	}

	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
		DrawQuad(model, color, -1.0f);
	}

	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
		DrawQuad(model, color, CalculateTextureIndex(texture));
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, -1.0f);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, CalculateTextureIndex(texture));
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, -1.0f);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, CalculateTextureIndex(texture));
	}

	float Renderer::CalculateTextureIndex(std::shared_ptr<Texture>& texture) {
		float texture_id = -1.0f;

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++) 
			if (*renderer_data.textures[i] == texture) 
				texture_id = (float)i;

		if (texture_id == -1.0f) {
			renderer_data.textures[renderer_data.texture_slot_index] = &texture;
			texture_id = (float) renderer_data.texture_slot_index;
			renderer_data.texture_slot_index++;

			if (renderer_data.texture_slot_index == MAX_TEXTURE_SLOTS)
				NewBatch();
		}

		return texture_id;
	}
}