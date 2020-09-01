#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "Renderer/VertexArray.h"
#include "OpenGLBuffer.h"

namespace Pixel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind();
		void UnBind();

		std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() { return vertex_buffers; };
		std::shared_ptr<IndexBuffer> GetIndexBuffers() { return index_buffer; }

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buf);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buf) { index_buffer = index_buf; }

		uint32_t GetId() const { return vertex_array_buffer_id; }
	private:
		uint32_t vertex_array_buffer_id;

		std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;

		std::shared_ptr<IndexBuffer> index_buffer;
	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
