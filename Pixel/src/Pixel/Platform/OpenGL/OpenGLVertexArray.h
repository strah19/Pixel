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
		uint32_t GetIndexBufferSize() const { return index_size; }

		void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertex_buf, VertexBufferFormat format);
		void SetIndexBufferSize(uint32_t index_buf) { index_size = index_buf; }

		virtual void EnableVertexAttrib(uint32_t index);
		virtual void SetArrayForInstancing(std::shared_ptr<VertexBuffer>& vertex_buf, uint32_t offset_sizes[], uint32_t stride_sizes[]);

		uint32_t GetId() const { return vertex_array_buffer_id; }
	private:
		uint32_t vertex_array_buffer_id;

		std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;

		uint32_t index_size = 0;
	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
