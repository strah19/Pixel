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
		std::shared_ptr<IndexBuffer> index_buffer;
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<Shader> shader;

		Vertex* vertices_base = nullptr;
		Vertex* vertices_ptr = nullptr;

		glm::vec4 quad_positions[4];
		uint32_t texture_slot_index = 0;
		std::array<std::shared_ptr<Texture>, MAX_TEXTURE_SLOTS> textures;
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

		uint32_t indices[MAX_INDEX_COUNT];
		uint32_t offset = 0;
		for (int i = 0; i < MAX_INDEX_COUNT; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;
			offset += 4;
		}

		renderer_data.vertices_base = new Vertex[MAX_VERTEX_COUNT];

		renderer_data.vertex_array->SetIndexBuffer(IndexBuffer::CreateIndexBuffer(indices, sizeof(indices)));
		renderer_data.vertex_array->AddVertexBuffer(renderer_data.vertex_buffer);

		renderer_data.shader = Shader::CreateShader();
		renderer_data.shader->Init("shaders/shader.glsl");

		renderer_data.quad_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		renderer_data.quad_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer::BeginScene(OrthoCamera& camera) {
		renderer_data.vertices_ptr = renderer_data.vertices_base;
		renderer_data.shader->Bind();
		renderer_data.shader->SetMat4f("proj", camera.GetProjection());
		renderer_data.shader->SetMat4f("view", camera.GetView());
		renderer_data.texture_slot_index = 0;
	}

	void Renderer::EndScene() {
		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++) {
			renderer_data.textures[i]->Bind(i);
		}
		auto loc = renderer_data.shader->GetUniformLocation("ourTexture");
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
			sampler[i] = i;
		glUniform1iv(loc, MAX_TEXTURE_SLOTS, sampler);

		uint32_t size = (uint32_t)((uint8_t*)renderer_data.vertices_ptr - (uint8_t*)renderer_data.vertices_base);

		if (size == 0)
			return;

		renderer_data.vertex_buffer->SetData(renderer_data.vertices_base, size);

		Renderer::Submit(renderer_data.vertex_array, renderer_data.shader);
	}

	Renderer::~Renderer() {
		delete[] renderer_data.vertices_base;

	}

	void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<Shader>& shader) {
		shader->Bind();
		vertex_array->Bind();
		vertex_array->GetIndexBuffers()->Bind();

		Pixel::RendererCommand::DrawVertexArray(vertex_array);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x + (size.x / 2), position.y + (size.y / 2), 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture) {
		renderer_data.textures[renderer_data.texture_slot_index] = texture;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x + (size.x / 2), position.y + (size.y / 2), 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, renderer_data.texture_slot_index);
	}

	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x + (size.x / 2), position.y + (size.y / 2), 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
		DrawQuad(model, color);
	}

	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, std::shared_ptr<Texture>& texture) {
		renderer_data.textures[renderer_data.texture_slot_index] = texture;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x + (size.x / 2), position.y + (size.y / 2), 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
		DrawQuad(model, color, renderer_data.texture_slot_index);
	}

	void Renderer::DrawQuad(const glm::mat4& translation, const glm::vec4& color, uint32_t texture_id) {
		for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
			renderer_data.vertices_ptr->position = translation * renderer_data.quad_positions[i];
			renderer_data.vertices_ptr->color = color;
			renderer_data.vertices_ptr->texture_coordinates = TEX_COORDS[i];
			renderer_data.vertices_ptr->texture_id = texture_id;
			renderer_data.vertices_ptr++;
		}
		renderer_data.texture_slot_index++;
	}

	void Renderer::DrawQuad(const glm::mat4& translation, const glm::vec4& color) {
		for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
			renderer_data.vertices_ptr->position = translation * renderer_data.quad_positions[i];
			renderer_data.vertices_ptr->color = color;
			renderer_data.vertices_ptr->texture_coordinates = TEX_COORDS[i];
			renderer_data.vertices_ptr->texture_id = -1;
			renderer_data.vertices_ptr++;
		}
	}
}