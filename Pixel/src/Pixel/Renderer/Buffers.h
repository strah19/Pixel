#ifndef BUFFER_H
#define BUFFER_H

#include <memory>

namespace Pixel {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size);
	};

	class IndexBuffer {  
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t size);
	};
}

#endif // !BUFFER_H
