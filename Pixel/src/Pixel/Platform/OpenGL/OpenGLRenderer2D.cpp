#include "pixelpch.h"
#include "OpenGLRenderer2D.h"

#include <glad/glad.h>

namespace Pixel {
	void OpeGLRenderer2D::BeginScene() {

	}

	void OpeGLRenderer2D::EndScene() {

	}

	void OpeGLRenderer2D::Submit(std::shared_ptr<VertexArray> vertex_array, std::shared_ptr<Shader> shader) {
		shader->Bind();
		vertex_array->Bind();
		vertex_array->GetIndexBuffers()->Bind();

		glDrawElements(GL_TRIANGLES, vertex_array->GetVertexBuffers().back()->GetLayout()->GetStride(), GL_UNSIGNED_INT, 0);
	}
}