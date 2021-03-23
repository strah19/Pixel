#include "pixelpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Pixel {
	static uint32_t current_index_buffer_id = 0;
	static uint32_t current_vertex_buffer_id = 0;

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
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size) {
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
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
	}
}