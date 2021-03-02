#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"

namespace Pixel {
	class OrthoCamera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);
		OrthoCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);

		inline glm::mat4 GetProjection() const { return projection_matrix; }
		inline glm::mat4 GetView() const { return view_matrix; }
		inline void SetMatrixProjection(const glm::mat4& projection) { projection_matrix = projection; }
		inline void SetMatrixView(const glm::mat4& view) { view_matrix = view; }

		inline void SetRotation(float r) { rotation = r; CalculateView(); }
		inline float GetRotation() const { return rotation; }

		inline glm::vec3 GetPosition() const { return position; }
		inline void SetPosition(const glm::vec3& pos) { position = pos; CalculateView(); }
	private:
		glm::vec3 position = { 0, 0, 0 };

		glm::mat4 projection_matrix;
		glm::mat4 view_matrix;

		float rotation = 0.0f;
		void CalculateView();
	};
}

#endif // !ORTHO_CAMERA_H
