#include "pixelpch.h"
#include "OrthoCamera.h"

namespace Pixel {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
		SetProjection(left, right, bottom, top);
	}

	void OrthoCamera::CalculateView() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		view_matrix = glm::inverse(transform);
	}
	void OrthoCamera::SetProjection(float left, float right, float bottom, float top) {
		projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		CalculateView();
	}
}