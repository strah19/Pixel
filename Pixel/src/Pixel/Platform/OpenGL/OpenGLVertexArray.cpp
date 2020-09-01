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

	void OpenGLVertexArray::SetVertexAttributes(VertexBufferLayout& buffer_layout) {
		uint32_t stride = buffer_layout.Calculate();
		for (auto& elements : buffer_layout.GetLayout()) {
			glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE, stride * sizeof(float),
			(void*)(elements.offset * sizeof(float)));

			glEnableVertexAttribArray(elements.index);
			std::cout << elements.index << " " << elements.offset << " " << elements.size << " " << stride << std::endl;
		}
	}
}