#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include "Buffers.h"

namespace Pixel {
	class VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buf) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buf) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffers() = 0;

		virtual uint32_t GetId() const = 0;

		static std::shared_ptr<VertexArray> CreateVertexArray();
	};
}

#endif // !VERTEX_ARRAY_H
