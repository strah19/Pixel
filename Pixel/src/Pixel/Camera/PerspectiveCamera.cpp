#include "pixelpch.h"
#include "PerspectiveCamera.h"

namespace Pixel {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio) {
		SetProjection(fov, aspect_ratio);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect_ratio) {
		CalculateView();
		projection_matrix = glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
	}

	void PerspectiveCamera::CalculateView() {
		view_matrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		view_matrix = glm::inverse(view_matrix);
	}
}