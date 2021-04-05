#include "pixelpch.h"
#include "Renderer/Buffers.h"
#include "Renderer/RendererCommands.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Pixel {
	std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(float* vertices, uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint32_t* indices, uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLIndexBuffer>(size);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::CreateUnifromBuffer(uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}