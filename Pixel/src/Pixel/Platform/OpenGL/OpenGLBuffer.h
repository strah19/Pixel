#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "Renderer/Buffers.h"

namespace Pixel {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind();
		void UnBind();
		void SetData(void* data, uint32_t size);

		uint32_t GetId() const { return vertex_buffer_id; }

		void SetLayout(const VertexBufferLayout& lay) { layout = std::make_shared<VertexBufferLayout>(lay); }
		std::shared_ptr<VertexBufferLayout> GetLayout() { return layout; }
	private:
		uint32_t vertex_buffer_id;
		std::shared_ptr<VertexBufferLayout> layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		OpenGLIndexBuffer(uint32_t size);
		virtual ~OpenGLIndexBuffer();
		void SetData(uint32_t* data, uint32_t size);

		void Bind();
		void UnBind();
		uint32_t GetId() const { return index_buffer_id; }
		uint32_t GetCount() const { return count; }
	private:
		uint32_t index_buffer_id;
		uint32_t count = 0;
	};
}

#endif // !OPENGL_BUFFER_H
