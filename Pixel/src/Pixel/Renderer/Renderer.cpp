#include "pixelpch.h"
#include "Renderer.h"
#include "Renderer/RendererCommands.h"

#include <array>
#include <gtc/matrix_transform.hpp>

namespace Pixel {
	glm::vec3 CalculateVertexNormals(const glm::vec3 triangle[]) {
		glm::vec3 norm = glm::cross((triangle[1] - triangle[0]), (triangle[2] - triangle[0]));
		return glm::normalize(norm);
	}

	void CalculateVertexNormalsAsRects(RenderMesh& mesh) {
		size_t count = 3;
		for (size_t i = 0; i < mesh.vertex_buffer_data.size(); i++) {
			if (i != 0 && i % count == 0) {
				glm::vec3 norm = glm::cross((mesh.vertex_buffer_data[i - 2].position - mesh.vertex_buffer_data[i - 3].position), (mesh.vertex_buffer_data[i - 1].position - mesh.vertex_buffer_data[i - 3].position));
				norm = glm::normalize(norm);
				mesh.vertex_buffer_data[i].normals = norm;
				mesh.vertex_buffer_data[i - 1].normals = norm;
				mesh.vertex_buffer_data[i - 2].normals = norm;
				mesh.vertex_buffer_data[i - 3].normals = norm;
				count += 4;
			}
		}
	}

	struct RendererData {
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;
		std::shared_ptr<UniformBuffer> uniform_buffer;
		std::shared_ptr<Shader>* current_shader;
		std::shared_ptr<Shader> default_shader;
		Material* material;

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
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));

		renderer_data.vertex_buffer->SetLayout(layout);

		renderer_data.index_buffer = IndexBuffer::CreateIndexBuffer(MAX_INDEX_COUNT * sizeof(uint32_t));
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());
		renderer_data.vertex_array->AddVertexBuffer(renderer_data.vertex_buffer);
		
		renderer_data.uniform_buffer = UniformBuffer::CreateUnifromBuffer(sizeof(glm::mat4));

		InitDefaultShader();
	}

	void Renderer::InitDefaultShader() {
		renderer_data.default_shader = Shader::CreateShader();
		renderer_data.default_shader->Init("shaders/shader.glsl");

		InitRendererShader(renderer_data.default_shader.get());
	}

	void Renderer::InitRendererShader(Shader* shader) {
		shader->Bind();
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
			sampler[i] = i;
		}
		shader->SetIntArray("ourTexture", sampler);
	}

	void Renderer::BeginScene(Camera& camera) {
		renderer_data.proj_view = camera.GetProjection() * camera.GetView();
		renderer_data.current_shader = &renderer_data.default_shader;
		renderer_data.material = nullptr;
		StartBatch();
	}

	void Renderer::EndScene() {
		Render();
	}

	uint32_t Renderer::GetShaderId() {
		return renderer_data.current_shader->get()->GetId();
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
		renderer_data.uniform_buffer->Bind();

		renderer_data.uniform_buffer->SetData((void*)&renderer_data.proj_view, sizeof(glm::mat4));

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
			renderer_data.textures[i]->get()->Bind(i);

		for (auto& mesh : renderer_data.meshes) {
			std::shared_ptr<Shader>* shader = mesh.shader;
			shader->get()->Bind();
			renderer_data.uniform_buffer->BindToShader(shader->get()->GetId(), "GlobalMatrices");

			if(mesh.material)
				mesh.material->PassToShader(shader, renderer_data.texture_slot_index);

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

	void Renderer::DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]) {
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
			vertex.texture_coordinates = tex_coords[i];
			vertex.texture_id = texture_id;
			current_mesh->vertex_buffer_data.push_back(vertex);

			renderer_data.num_of_vertices_in_batch++;
		}
		CalculateVertexNormalsAsRects(*current_mesh);

		if (renderer_data.num_of_vertices_in_batch == MAX_VERTEX_COUNT)
			NewBatch();
	}

	void Renderer::SetShader(std::shared_ptr<Shader>* shader) {
		renderer_data.current_shader = shader;
	}

	void Renderer::SetMaterial(Material* material) {
		renderer_data.material = material;
	}

	void Renderer::SetShaderToDefualt() {
		renderer_data.current_shader = &renderer_data.default_shader;
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, -1.0f, TEX_COORDS);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, CalculateTextureIndex(texture), TEX_COORDS);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, -1.0f, tex_coords);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec2 tex_coords[], const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(model, color, CalculateTextureIndex(texture), tex_coords);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, -1.0f, TEX_COORDS);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, CalculateTextureIndex(texture), TEX_COORDS);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, -1.0f, tex_coords);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		model = glm::rotate(model, glm::radians(rotation), rotation_orientation);
		DrawQuad(model, color, CalculateTextureIndex(texture), tex_coords);
	}

	void Renderer::AddMesh(RenderMesh& mesh) {
		bool need_new_mesh = true;
		for (auto& in_mesh : renderer_data.meshes) {
			if (in_mesh.shader == mesh.shader || in_mesh.material == mesh.material) {
				need_new_mesh = false;
				in_mesh.indices.insert(in_mesh.indices.end(), mesh.indices.begin(), mesh.indices.end());
				in_mesh.vertex_buffer_data.insert(in_mesh.vertex_buffer_data.end(), mesh.vertex_buffer_data.begin(), mesh.vertex_buffer_data.end());
			}
		}

		if (need_new_mesh) {
			renderer_data.meshes.push_back(mesh);
			renderer_data.current_shader = mesh.shader;
			renderer_data.material = mesh.material;
		}
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		DrawCube(model, color, -1.0f, TEX_COORDS);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		DrawCube(model, color, CalculateTextureIndex(texture), TEX_COORDS);
	}

	void Renderer::DrawCube(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]) {
		RenderMesh* current_mesh = FindMesh();
		for (uint32_t i = 0; i < 6; i++)
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
			vertex.position = translation * glm::vec4(CUBE_POSITIONS[i].x, CUBE_POSITIONS[i].y, CUBE_POSITIONS[i].z, 1.0f);
			vertex.color = color;
			vertex.texture_coordinates = tex_coords[i % 4];
			vertex.texture_id = texture_id;

			current_mesh->vertex_buffer_data.push_back(vertex);

			renderer_data.num_of_vertices_in_batch++;
		}
		CalculateVertexNormalsAsRects(*current_mesh);

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
				 if (mesh.shader == renderer_data.current_shader || mesh.material == renderer_data.material) {
					 current_mesh = &mesh;
					 break;
				 }
		 }
		 if (!current_mesh) {
			 renderer_data.meshes.push_back(RenderMesh());
			 current_mesh = &renderer_data.meshes.back();
			 current_mesh->shader = renderer_data.current_shader;
			 current_mesh->material = renderer_data.material;
			 renderer_data.index_offset = 0;
		 }

		 return current_mesh;
	}
}