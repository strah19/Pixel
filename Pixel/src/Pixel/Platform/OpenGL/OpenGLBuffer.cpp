#include "pixelpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Pixel {
	static uint32_t current_index_buffer_id = 0;
	static uint32_t current_vertex_buffer_id = 0;
	static uint32_t current_uniform_buffer_id = 0;

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		glGenBuffers(1, &vertex_buffer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		glGenBuffers(1, &vertex_buffer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &vertex_buffer_id);
	}

	void OpenGLVertexBuffer::Bind() {
		if (current_vertex_buffer_id != vertex_buffer_id) {
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
			current_index_buffer_id = vertex_buffer_id;
		}
	}

	void OpenGLVertexBuffer::UnBind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size) {
		glGenBuffers(1, &index_buffer_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		count = size / sizeof(*indices);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size) {
		glGenBuffers(1, &index_buffer_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		count = 0;
	}

	void OpenGLIndexBuffer::SetData(uint32_t* data, uint32_t size) {
		Bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
		count = size / sizeof(*data);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &index_buffer_id);
	}

	void OpenGLIndexBuffer::Bind() {
		if (current_index_buffer_id != index_buffer_id) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
			current_index_buffer_id = index_buffer_id;
		}
	}

	void OpenGLIndexBuffer::UnBind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	static uint32_t uniform_buffer_point_latest = 0;
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size) {
		glGenBuffers(1, &uniform_buffer_id);
		uniform_buffer_point = uniform_buffer_point_latest++;
		Bind();
		AllocateData(size);
		size_of_buffer = size;
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &uniform_buffer_id);
	}

	void OpenGLUniformBuffer::Bind() {
		if (current_uniform_buffer_id != uniform_buffer_id) {
			glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_id);
			current_uniform_buffer_id = uniform_buffer_id;
		}
	}

	void OpenGLUniformBuffer::UnBind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	uint32_t OpenGLUniformBuffer::GetId() const {
		return uniform_buffer_id;
	}

	void OpenGLUniformBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	uint32_t OpenGLUniformBuffer::GetUniformBlockId(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void OpenGLUniformBuffer::BindToShader(uint32_t shader_id, const std::string& block_name) {
		glUniformBlockBinding(shader_id, GetUniformBlockId(shader_id, block_name), uniform_buffer_point);
		BindToBindPoint();
	}

	void OpenGLUniformBuffer::BindToBindPoint() {
		glBindBufferRange(GL_UNIFORM_BUFFER, uniform_buffer_point, uniform_buffer_id, 0, size_of_buffer);
	}

	void OpenGLUniformBuffer::AllocateData(uint32_t size) {
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	static uint32_t current_indirect_draw_buffer = 0;
	OpenGLIndirectDrawBuffer::OpenGLIndirectDrawBuffer(uint32_t size) {
		glGenBuffers(1, &indirect_buffer_id);
		Bind();
		AllocateData(size, nullptr);
		size_of_buffer = size;
	}

	OpenGLIndirectDrawBuffer::~OpenGLIndirectDrawBuffer() {
		glDeleteBuffers(1, &indirect_buffer_id);
	}

	void OpenGLIndirectDrawBuffer::Bind() {
		if (current_indirect_draw_buffer != indirect_buffer_id) {
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer_id);
			current_indirect_draw_buffer = indirect_buffer_id;
		}
	}

	void OpenGLIndirectDrawBuffer::UnBind() {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}

	uint32_t OpenGLIndirectDrawBuffer::GetId() const {
		return indirect_buffer_id;
	}

	void OpenGLIndirectDrawBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
	}

	void OpenGLIndirectDrawBuffer::AllocateData(uint32_t size, void* data) {
		glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	static uint32_t shader_buffer_point_latest = 0;
	static uint32_t current_shader_storage_id = 0;
	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size) {
		glGenBuffers(1, &shader_storage_id);
		Bind();
		AllocateData(size, nullptr);
		size_of_buffer = size;
		binding_point = shader_buffer_point_latest++;
	}

	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer() {
		glDeleteBuffers(1, &shader_storage_id);
	}

	void OpenGLShaderStorageBuffer::Bind() {
		if (current_shader_storage_id != shader_storage_id) {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader_storage_id);
			current_shader_storage_id = shader_storage_id;
		}
	}

	void OpenGLShaderStorageBuffer::UnBind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	uint32_t OpenGLShaderStorageBuffer::GetId() const {
		return shader_storage_id;
	}

	void OpenGLShaderStorageBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	}

	void OpenGLShaderStorageBuffer::AllocateData(uint32_t size, void* data) {
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); 
	}

	uint32_t OpenGLShaderStorageBuffer::GetUniformBlockId(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void OpenGLShaderStorageBuffer::BindToShader(uint32_t shader_id, const std::string& block_name) {
		glShaderStorageBlockBinding(shader_id, GetUniformBlockId(shader_id, block_name), binding_point);
		BindToBindPoint();
	}

	void OpenGLShaderStorageBuffer::BindToBindPoint() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, shader_storage_id);
	}
}