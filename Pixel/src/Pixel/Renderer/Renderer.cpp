#include "pixelpch.h"
#include "Renderer.h"
#include "Renderer/RendererCommands.h"
#include <gtc/matrix_transform.hpp>

namespace Pixel {
	glm::vec3 CalculateVertexNormals(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
		glm::vec3 norm = glm::cross((b - a), (c - a));
		return glm::normalize(norm);
	}

	struct RendererData {
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
		Camera* camera;

		uint32_t num_of_vertices_in_batch = 0;

		DrawElementsCommand draw_commands[MAX_DRAW_COMMANDS];
		uint32_t base_vert = 0;
		uint32_t draw_count = 0;
		uint32_t current_draw_command_vertex_size = 0;

		Vertex* vertices_base = nullptr;
		Vertex* vertices_ptr = nullptr;

		uint32_t* index_base = nullptr;
		uint32_t* index_ptr = nullptr;
		uint32_t current_material_id = -1;
	};

	static RendererData renderer_data;

	void Renderer::Init() {
		renderer_data.vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(Vertex) * MAX_VERTEX_COUNT);
		renderer_data.vertex_array = VertexArray::CreateVertexArray();

		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));

		renderer_data.vertex_buffer->SetLayout(layout);

		renderer_data.vertices_base = new Vertex[MAX_VERTEX_COUNT];
		renderer_data.index_base = new uint32_t[MAX_INDEX_COUNT];

		renderer_data.index_buffer = IndexBuffer::CreateIndexBuffer(MAX_INDEX_COUNT * sizeof(uint32_t));
		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());
		renderer_data.vertex_array->AddVertexBuffer(renderer_data.vertex_buffer, VertexBufferFormat::VNCVNCVNC);
		
		renderer_data.indirect_draw_buffer = IndirectDrawBuffer::CreateIndirectDrawBuffer(sizeof(renderer_data.draw_commands));

		renderer_data.default_shader.Init("shaders/default_shader.glsl");

		renderer_data.default_shader.AddSSBOReference("GlobalMatrices", sizeof(glm::mat4));
	}
	
	void Renderer::Destroy() {
		delete[] renderer_data.vertices_base;
		delete[] renderer_data.index_base;
	}

	void Renderer::InitRendererShader(Shader* shader) {
		shader->Bind();
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
			sampler[i] = i;
		}
		shader->SetIntArray("textures", sampler);
	}

	void Renderer::BeginScene(Camera& camera, RenderFlags flags) {
		Pixel::SSBOManager::GetSSBOManager()->QueueForNewFrame();
		if (flags & RenderFlags::SkyBoxFlag) {
			glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));
			renderer_data.proj_view = camera.GetProjection() * view;
		}
		else 
			renderer_data.proj_view = camera.GetProjection() * camera.GetView();
		
		renderer_data.camera = &camera;
		renderer_data.current_shader = &renderer_data.default_shader;
		renderer_data.current_material_id = -1;
		StartBatch();
	}

	void Renderer::EndScene() {
		Render();
	}

	uint32_t Renderer::GetShaderId() {
		return renderer_data.current_shader->GetShader()->get()->GetId();
	}

	void Renderer::StartBatch() {
		renderer_data.texture_slot_index = 0;
		renderer_data.num_of_vertices_in_batch = 0;
		renderer_data.index_offset = 0;

		renderer_data.base_vert = 0;
		renderer_data.draw_count = 0;
		renderer_data.current_draw_command_vertex_size = 0;

		renderer_data.vertices_ptr = renderer_data.vertices_base;
		renderer_data.index_ptr = renderer_data.index_base;
	}

	void Renderer::Render() {
		renderer_data.vertex_array->Bind(); 
		renderer_data.index_buffer->Bind();
		renderer_data.vertex_buffer->Bind();

		renderer_data.indirect_draw_buffer->Bind();
		renderer_data.indirect_draw_buffer->SetData(renderer_data.draw_commands, sizeof(renderer_data.draw_commands), 0);

		std::shared_ptr<Shader>* shader = renderer_data.current_shader->GetShader();
		shader->get()->Bind();

		SSBOData* ssbo = renderer_data.default_shader.GetBufferPointer("GlobalMatrices");
		ssbo->ssbo->Bind();
		renderer_data.current_shader->UpdateSSBO(ssbo, 0, (void*)&renderer_data.proj_view, sizeof(glm::mat4));
		renderer_data.current_shader->SSBOUploadFinised(ssbo);
		ssbo->ssbo->BindToBindPoint();

		for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
			renderer_data.textures[i]->get()->Bind(i);

		uint32_t vertex_buf_size = (uint32_t)((uint8_t*)renderer_data.vertices_ptr - (uint8_t*)renderer_data.vertices_base);
		uint32_t index_buf_size = (uint32_t)((uint8_t*)renderer_data.index_ptr - (uint8_t*)renderer_data.index_base);

		renderer_data.vertex_buffer->SetData(renderer_data.vertices_base, vertex_buf_size);
		renderer_data.index_buffer->SetData(renderer_data.index_base, index_buf_size);

		renderer_data.vertex_array->SetIndexBufferSize(renderer_data.index_buffer->GetCount());

		RendererCommand::DrawMultiIndirect(nullptr, renderer_data.draw_count + 1, 0);
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
		if (renderer_data.num_of_vertices_in_batch + QUAD_VERTEX_COUNT > MAX_VERTEX_COUNT)
			NewBatch();

		CalculateSquareIndices();

		glm::vec3 norm = CalculateVertexNormals(translation * QUAD_POSITIONS[0], translation * QUAD_POSITIONS[1], translation * QUAD_POSITIONS[2]);

		for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
			Vertex vertex;
			vertex.position = translation * QUAD_POSITIONS[i];
			vertex.color = color;
			vertex.texture_coordinates = tex_coords[i];
			vertex.texture_id = texture_id;
			vertex.material_id = (float) renderer_data.current_material_id;
			vertex.normals = norm;

			*renderer_data.vertices_ptr = vertex;
			renderer_data.vertices_ptr++;

			renderer_data.num_of_vertices_in_batch++;
		}

		renderer_data.current_draw_command_vertex_size += 6;
	}

	void Renderer::CalculateSquareIndices() {
		*renderer_data.index_ptr = renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 1 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 2 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 2 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = 3 + renderer_data.index_offset;
		renderer_data.index_ptr++;
		*renderer_data.index_ptr = renderer_data.index_offset;
		renderer_data.index_ptr++;

		renderer_data.index_offset += 4;   
	}

	void Renderer::GoToNextDrawCommand() {
		renderer_data.draw_count++;
		renderer_data.base_vert += renderer_data.num_of_vertices_in_batch;
		renderer_data.current_draw_command_vertex_size = 0;
	}

	void Renderer::MakeCommand() {
		renderer_data.draw_commands[renderer_data.draw_count].vertex_count = renderer_data.current_draw_command_vertex_size;
		renderer_data.draw_commands[renderer_data.draw_count].instance_count = 1;
		renderer_data.draw_commands[renderer_data.draw_count].first_index = 0;
		renderer_data.draw_commands[renderer_data.draw_count].base_vertex = renderer_data.base_vert;
		renderer_data.draw_commands[renderer_data.draw_count].base_instance = renderer_data.draw_count;
	}

	void Renderer::SetShader(std::shared_ptr<Shader>* shader) {
		renderer_data.current_shader->SetShader(shader);
	}

	void Renderer::SetShaderToDefualt() {
		renderer_data.current_shader = &renderer_data.default_shader;
	}

	void Renderer::SetMaterialId(uint32_t material_id) {
		renderer_data.current_material_id = material_id;
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

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		DrawCube(model, color, -1.0f, TEX_COORDS);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, std::shared_ptr<Texture>& texture, const glm::vec4& color) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		DrawCube(model, color, CalculateTextureIndex(texture), TEX_COORDS);
	}

	void Renderer::DrawCube(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]) {
		if (renderer_data.num_of_vertices_in_batch + CUBE_VERTEX_COUNT > MAX_VERTEX_COUNT)
			NewBatch(); 

		for (uint32_t i = 0; i < CUBE_FACES; i++)
			CalculateSquareIndices();
		
		Vertex* start_base = renderer_data.vertices_ptr;
		uint32_t face = -4;
		for (size_t i = 0; i < CUBE_VERTEX_COUNT; i++) {
			if (i % 4 == 0) {
				face += 4;
				Vertex* face_base_ptr = &start_base[face];

				glm::vec3 norm = CalculateVertexNormals(face_base_ptr[0].position, face_base_ptr[1].position, face_base_ptr[2].position);
				face_base_ptr[0].normals = norm;
				face_base_ptr[1].normals = norm;
				face_base_ptr[2].normals = norm;
				face_base_ptr[3].normals = norm;

			}
		
			Vertex vertex;
			vertex.position = translation * glm::vec4(CUBE_POSITIONS[i].x, CUBE_POSITIONS[i].y, CUBE_POSITIONS[i].z, 1.0f);
			vertex.color = color;
			vertex.texture_coordinates = tex_coords[i % 4];
			vertex.texture_id = texture_id;
			vertex.material_id = (float) renderer_data.current_material_id;

			*renderer_data.vertices_ptr = vertex;
			renderer_data.vertices_ptr++;

			renderer_data.num_of_vertices_in_batch++;
		}

		renderer_data.current_draw_command_vertex_size += 36;
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
}