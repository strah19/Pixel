#ifndef BUFFER_H
#define BUFFER_H

#include <memory>

namespace Pixel {
	class VertexBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices);
	};

	class IndexBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t indices_count);
	};
}

#endif // !BUFFER_H
