#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include "PerspectiveCamera.h"
#include "Events/Events.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "Events/KeyboardEvents.h"

namespace Pixel {
	class PerspectiveCameraController {
	public:
		PerspectiveCameraController() = default;
		PerspectiveCameraController(glm::vec2& window_size);
		PerspectiveCamera& GetCamera() { return camera; }
		void OnEvent(Event& event);
		void Update();
		void SetFreeze(bool f);

		inline glm::vec3 GetFrontCamera() const { return camera_front; }
	private:
		bool MouseWheelHandler(MouseWheelEvent& mousewheel);
		bool KeyboardHandler(KeyboardEvents& keyboard);
		bool WindowResizeHandler(ResizeEvent& resize);
		float aspect_ratio = 0.0f;
		PerspectiveCamera camera;
		float fov = 60.0f;

		glm::vec3 camera_pos = glm::vec3(0, 0, 3 );
		glm::vec3 camera_target = glm::vec3(0, 0, 0);
		glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

		float yaw = -90.0f, pitch = 0.0f;
		glm::vec2 last_mouse_position = { 0, 0 };
		glm::vec3 latest_camera_position = { 0, 0, 0 };

		glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

		bool in_camera_mode = false;
		bool freeze = false;
	};
}

#endif // !PERSPECTIVE_CAMERA_CONTROLLER_H
