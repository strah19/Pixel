#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "Renderer/Buffers.h"

namespace Pixel {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind();
		void UnBind();
	private:
		uint32_t vertex_buffer_id;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		void Bind();
		void UnBind();
	private:
		uint32_t index_buffer_id;
	};
}

#endif // !OPENGL_BUFFER_H
