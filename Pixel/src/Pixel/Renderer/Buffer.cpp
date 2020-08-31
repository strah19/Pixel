#include "pixelpch.h"
#include "Renderer/Buffers.h"

namespace Pixel {
	std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(float* vertices) {
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint32_t* indices, uint32_t size) {
		return nullptr;
	}
}