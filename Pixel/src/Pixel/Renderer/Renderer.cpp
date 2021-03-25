#include "pixelpch.h"
#include "Renderer.h"
#include "Renderer/RendererCommands.h"

#include <glad/glad.h>
#include <array>
#include <gtc/matrix_transform.hpp>

namespace Pixel {
	constexpr size_t MAX_QUAD_COUNT = 1000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * QUAD_VERTEX_COUNT;
	constexpr size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	constexpr size_t MAX_TEXTURE_SLOTS = 32;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr glm::vec4 QUAD_POSITIONS[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};
	constexpr size_t CUBE_VERTEX_COUNT = 8;
	constexpr glm::vec3 CUBE_POSITIONS[CUBE_VERTEX_COUNT] = {
		{-1.0, -1.0,  1.0},
		{1.0, -1.0,  1.0},
		{1.0,  1.0,  1.0},
		{-1.0,  1.0,  1.0},

		{-1.0, -1.0, -1.0},
		{1.0, -1.0, -1.0},
		{1.0,  1.0, -1.0},
		{-1.0,  1.0, -1.0}
	};

	struct RendererData {
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;
		std::shared_ptr<Shader>* current_shader;
		std::shared_ptr<Shader> default_shader;
		std::function<void(Shader* shader)> shader_uniform_func;
		bool init_default_shader = false;

		uint32_t index_offset = 0;

		uint32_t texture_slot_index = 0;
		std::shared_ptr<Texture>* textures[MAX_TEXTURE_SLOTS];
		glm::mat4 proj_view = glm::mat4(1.0f);

		uint32_t num_of_vertices_in_batch = 0;

		std::vector<RenderMesh> meshes;
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

		renderer_data.index_buffer = IndexBuffer::CreateIndexBuffer(MAX_INDEX_COUNT * sizeof(uint32_t));
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());
		renderer_data.vertex_array->AddVertexBuffer(renderer_data.vertex_buffer);
	}

	void Renderer::InitDefaultShader() {
		renderer_data.default_shader = Shader::CreateShader();
		renderer_data.default_shader->Init("shaders/shader.glsl");

		InitRendererShader(renderer_data.default_shader.get());
		renderer_data.init_default_shader = true;
	}

	void Renderer::InitRendererShader(Shader* shader) {
		shader->Bind();
		auto loc = shader->GetUniformLocation("ourTexture");
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
			sampler[i] = i;
		}
		glUniform1iv(loc, MAX_TEXTURE_SLOTS, sampler);
	}

	void Renderer::BeginScene(Camera& camera) {
		renderer_data.proj_view = camera.GetProjection() * camera.GetView();
		renderer_data.current_shader = (renderer_data.init_default_shader) ? &renderer_data.default_shader : nullptr;
		renderer_data.shader_uniform_func = [](Shader* shader) { };
		StartBatch();
	}

	void Renderer::EndScene() {
		Render();
	}

	void Renderer::StartBatch() {
		renderer_data.texture_slot_index = 0;
		renderer_data.num_of_vertices_in_batch = 0;
		renderer_data.index_offset = 0;
		renderer_data.meshes.clear();
	}

	void Renderer::Render() {
		renderer_data.vertex_array->Bind();
		renderer_data.index_buffer->Bind();
		renderer_data.vertex_buffer->Bind();

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
			renderer_data.textures[i]->get()->Bind(i);
		for (auto& mesh : renderer_data.meshes) {
			std::shared_ptr<Shader>* shader = mesh.shader;
			shader->get()->Bind();
			shader->get()->SetMat4f("proj_view", renderer_data.proj_view);
			mesh.shader_uniforms(shader->get());

			renderer_data.vertex_buffer->SetData(&mesh.vertex_buffer_data[0], (uint32_t)mesh.vertex_buffer_data.size() * sizeof(Vertex));
			renderer_data.index_buffer->SetData(&mesh.indices[0], (uint32_t)mesh.indices.size() * sizeof(uint32_t));

			renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());

			RendererCommand::DrawVertexArray(renderer_data.vertex_array);	
		}
	}

	void Renderer::NewBatch() {
		Render();
		StartBatch();
	}

	void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, std::shared_ptr<IndexBuffer>& index_buffer, std::shared_ptr<Shader>& shader) {
		shader->Bind();
		vertex_array->Bind();
		index_buffer->Bind();

		Pixel::RendererCommand::DrawVertexArray(vertex_array);
	}

	void Renderer::DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id) {
		RenderMesh* current_mesh = FindMesh();

		current_mesh->indices.push_back(0 + renderer_data.index_offset);
		current_mesh->indices.push_back(1 + renderer_data.index_offset);
		current_mesh->indices.push_back(2 + renderer_data.index_offset);
		current_mesh->indices.push_back(2 + renderer_data.index_offset);
		current_mesh->indices.push_back(3 + renderer_data.index_offset);
		current_mesh->indices.push_back(0 + renderer_data.index_offset);
		renderer_data.index_offset += 4;

		for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
			Vertex vertex;
			vertex.position = translation * QUAD_POSITIONS[i];
			vertex.color = color;
			vertex.texture_coordinates = TEX_COORDS[i];
			vertex.texture_id = texture_id;
			current_mesh->vertex_buffer_data.push_back(vertex);
			renderer_data.num_of_vertices_in_batch++;
		}
		if (renderer_data.num_of_vertices_in_batch == MAX_VERTEX_COUNT)
			NewBatch();
	}

	void Renderer::SetShader(std::shared_ptr<Shader>* shader) {
		renderer_data.current_shader = shader;
	}

	void Renderer::AddUniformsToShader(const std::function<void(Shader* shader)>& shader_uniform_func) {
		renderer_data.shader_uniform_func = shader_uniform_func;
	}

	void Renderer::SetShaderToDefualt() {
		renderer_data.current_shader = &renderer_data.default_shader;
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

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		RenderMesh* current_mesh = FindMesh();

		for (uint32_t i = 0; i < 36; i += 6)
		{
			current_mesh->indices.push_back(0 + renderer_data.index_offset);
			current_mesh->indices.push_back(1 + renderer_data.index_offset);
			current_mesh->indices.push_back(2 + renderer_data.index_offset);
			current_mesh->indices.push_back(2 + renderer_data.index_offset);
			current_mesh->indices.push_back(3 + renderer_data.index_offset);
			current_mesh->indices.push_back(0 + renderer_data.index_offset);

			renderer_data.index_offset += 4;
		}

		for (size_t i = 0; i < CUBE_VERTEX_COUNT; i++) {
			Vertex vertex;
			vertex.position = model * glm::vec4(CUBE_POSITIONS[i].x, CUBE_POSITIONS[i].y, CUBE_POSITIONS[i].z, 1.0f);
			vertex.color = color;
			vertex.texture_coordinates = { 0, 0 };
			vertex.texture_id = -1.0f;
			current_mesh->vertex_buffer_data.push_back(vertex);
			renderer_data.num_of_vertices_in_batch++;
		}
		if (renderer_data.num_of_vertices_in_batch == MAX_VERTEX_COUNT)
			NewBatch();
	}

	float Renderer::CalculateTextureIndex(std::shared_ptr<Texture>& texture) {
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

	RenderMesh* Renderer::FindMesh() {
		 RenderMesh* current_mesh = nullptr;
		 if (!renderer_data.meshes.empty()) {
			 for (auto& mesh : renderer_data.meshes)
				 if (mesh.shader == renderer_data.current_shader) {
					 current_mesh = &mesh;
					 break;
				 }
		 }
		 if (!current_mesh) {
			 renderer_data.meshes.push_back(RenderMesh());
			 current_mesh = &renderer_data.meshes.back();
			 current_mesh->shader = renderer_data.current_shader;
			 renderer_data.index_offset = 0;
			 current_mesh->shader_uniforms = renderer_data.shader_uniform_func;
		 }

		 return current_mesh;
	}
}