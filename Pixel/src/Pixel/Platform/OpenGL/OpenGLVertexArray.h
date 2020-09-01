#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "Renderer/VertexArray.h"

namespace Pixel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind();
		void UnBind();

		void SetVertexAttributes(VertexBufferLayout& buffer_layout);
	private:
		uint32_t vertex_array_buffer_id;
	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
