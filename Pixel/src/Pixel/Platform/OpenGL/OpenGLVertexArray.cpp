#include "pixelpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Pixel {
	static GLenum VertexShaderTypeToOpenGL(VertexShaderType type) {
		switch (type) {
		case VertexShaderType::Float: return GL_FLOAT;
		case VertexShaderType::None: return GL_NONE;
		}
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &vertex_array_buffer_id);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &vertex_array_buffer_id);
	}

	void OpenGLVertexArray::Bind(){
		glBindVertexArray(vertex_array_buffer_id);
	}

	void OpenGLVertexArray::UnBind(){
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buf) {
		uint32_t stride = vertex_buf->GetLayout()->Calculate();

		vertex_buf->Bind();
		index_buffer->Bind();
		Bind();

		for (auto& elements : vertex_buf->GetLayout()->GetLayout()) {
			glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE, 
				stride * GetSizeInBytes(elements.type),
				(void*)(elements.offset * GetSizeInBytes(elements.type)));

			glEnableVertexAttribArray(elements.index);
		}

		vertex_buffers.push_back(vertex_buf);
	}
}