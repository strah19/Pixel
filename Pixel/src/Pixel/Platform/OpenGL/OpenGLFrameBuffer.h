#ifndef OPENGL_FRAME_BUFFER_H
#define OPENGL_FRAME_BUFFER_H

#include "Renderer/FrameBuffer.h"

namespace Pixel {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFrameBuffer();

		void Bind();
		void UnBind();
		uint32_t GetColorAttachment() { return color_attachment; }
		virtual uint32_t GetBufferStencilAttachment() const { return depth_stencil_attachment; }
	private:
		uint32_t frame_buffer_id;
		uint32_t color_attachment;
		uint32_t depth_stencil_attachment;
	};
}

#endif // !OPENGL_FRAME_BUFFER_H
