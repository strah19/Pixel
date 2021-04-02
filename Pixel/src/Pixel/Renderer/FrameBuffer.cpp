#include "pixelpch.h"
#include "RendererAPI.h"
#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Pixel {
	std::shared_ptr<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height) {
		switch (RendererAPI::GetAPI()) {
		case RenderAPI::OpenGL: return std::make_shared<OpenGLFrameBuffer>(width, height);
		case RenderAPI::None: return nullptr;
		}
		return nullptr;
	}
}