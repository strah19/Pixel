#ifndef ORTHO_CAMERA_CONTROLLER_H
#define ORTHO_CAMERA_CONTROLLER_H

#include "OrthoCamera.h"
#include "Events/Events.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

namespace Pixel {
	class OrthoCameraController {
	public:
		OrthoCameraController() = default;
		OrthoCameraController(glm::vec2& window_size);
		OrthoCamera& GetCamera() { return camera; }
		void OnEvent(Event& event);
		void Update();
	private:
		bool MouseWheelHandler(MouseWheelEvent& mousewheel);
		bool WindowResizeHandler(ResizeEvent& resize);
	private:
		OrthoCamera camera;

		float aspect_ratio = 0.0f;
		float zoom = -1.0f;
		glm::vec3 camera_pos = { 0, 0, 0 };
		glm::vec2 camera_speed = glm::vec2(2.0f, 2.0f);
	};
}

#endif // !ORTHO_CAMERA_CONTROLLER_H
