#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"

namespace Pixel{
	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov, float aspect_ratio);
		PerspectiveCamera() = default;

		void SetProjection(float fov, float aspect_ratio);

		inline glm::mat4 GetProjection() const { return projection_matrix; }
		inline glm::mat4 GetView() const { return view_matrix; }
		inline void SetMatrixProjection(const glm::mat4& projection) { projection_matrix = projection; }
		inline void SetMatrixView(const glm::mat4& view) { view_matrix = view; }

		inline void SetRotation(float r) { rotation = r; CalculateView(); }
		inline float GetRotation() const { return rotation; }

		inline glm::vec3 GetPosition() const { return position; }
		inline void SetPosition(const glm::vec3& pos) { position = pos; CalculateView(); }
	private:
		glm::vec3 position = { 0, 0, 3 };

		glm::mat4 projection_matrix = glm::mat4(1.0f);
		glm::mat4 view_matrix = glm::mat4(1.0f);

		float rotation = 0.0f;
		void CalculateView();
	};
}

#endif // !PERSPECTIVE_CAMERA_H
