#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>

namespace Pixel {
	class VertexArray {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<VertexArray> CreateVertexArray();
	};

	class VertexArrayLayout {
	public:

	};
}

#endif // !VERTEX_ARRAY_H
