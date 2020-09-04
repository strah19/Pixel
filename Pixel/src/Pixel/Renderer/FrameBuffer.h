#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

namespace Pixel {
	class FrameBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	private:

	};
}

#endif // !FRAME_BUFFER_H
