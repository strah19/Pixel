#include "pixelpch.h"
#include "PerspectiveCameraController.h"
#include "Events/EventHandler.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Pixel {
	constexpr float SMALLEST_ZOOM = 1.0f;
	constexpr float HIGHEST_ZOOM = 60.0f;
	constexpr float CAMERA_SPEED = 0.2f;

	PerspectiveCameraController::PerspectiveCameraController(glm::vec2& window_size) {
		aspect_ratio = (float)window_size.x / (float)window_size.y;
		camera = PerspectiveCamera(glm::radians(fov), aspect_ratio);
		camera_pos = camera.GetPosition();
		last_mouse_position = { window_size.x / 2, window_size.y / 2 };
		latest_camera_position = camera_pos;
	}

	void PerspectiveCameraController::Update() {
		if (!freeze) {
			if (latest_camera_position != camera_pos)
				camera.SetPosition(camera_pos);
			camera.SetMatrixView(glm::lookAt(camera_pos, camera_pos + camera_front, camera_up));

			latest_camera_position = camera_pos;

				if (EventHandler::KeyPressed(PIXEL_KEY_D))
				camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * CAMERA_SPEED;
				if (EventHandler::KeyPressed(PIXEL_KEY_A))
					camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * CAMERA_SPEED;
				if (EventHandler::KeyPressed(PIXEL_KEY_W))
					camera_pos += CAMERA_SPEED * camera_front;
				if (EventHandler::KeyPressed(PIXEL_KEY_S))
					camera_pos -= CAMERA_SPEED * camera_front;

			float xoffset = EventHandler::GetMouseCoordinates().x - last_mouse_position.x;
			float yoffset = last_mouse_position.y - EventHandler::GetMouseCoordinates().y;
			last_mouse_position = EventHandler::GetMouseCoordinates();

			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camera_front = glm::normalize(direction);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!freeze) {
			dispatcher.Dispatch<KeyboardEvents>(PIXEL_BIND_EVENT(KeyboardHandler));
			dispatcher.Dispatch<MouseWheelEvent>(PIXEL_BIND_EVENT(MouseWheelHandler));
		}
		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(WindowResizeHandler));
	}

	bool PerspectiveCameraController::MouseWheelHandler(MouseWheelEvent& mousewheel) {
		fov -= mousewheel.yoffset;
		if (fov < SMALLEST_ZOOM)
			fov = SMALLEST_ZOOM;
		if (fov > HIGHEST_ZOOM)
			fov = HIGHEST_ZOOM;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
		return true;
	}

	bool PerspectiveCameraController::WindowResizeHandler(ResizeEvent& resize) {
		aspect_ratio = (float)resize.width / (float)resize.height;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
		return true;
	}

	void PerspectiveCameraController::SetFreeze(bool f) {
		freeze = f;

		if(freeze)
			glfwSetInputMode(static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	bool PerspectiveCameraController::KeyboardHandler(KeyboardEvents& keyboard) {
		if (keyboard.key == PIXEL_KEY_M && keyboard.action == GLFW_PRESS) {
			in_camera_mode = !in_camera_mode;
			last_mouse_position = EventHandler::GetMouseCoordinates();
			if (!in_camera_mode)
				glfwSetInputMode(static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		return true;
	}
}