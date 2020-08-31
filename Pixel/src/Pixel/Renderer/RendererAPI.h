#ifndef RENDERER_API_H
#define RENDERER_API_H

namespace Pixel {
	enum class RenderAPI {
		OpenGL, None
	};

	class RendererAPI {
	public:
		static RenderAPI GetAPI() { return API; }
	private:
		static RenderAPI API;
	};
}

#endif // !RENDERER_API_H
