#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

namespace Pixel {
	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetColorAttachment() = 0;
		virtual uint32_t GetBufferStencilAttachment() const = 0;

		static std::shared_ptr<FrameBuffer> Create(uint32_t width, uint32_t height);
	private:
	};
}

#endif // !FRAME_BUFFER_H
