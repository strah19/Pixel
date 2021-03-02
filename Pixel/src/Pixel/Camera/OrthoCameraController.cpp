#include "pixelpch.h"
#include "OrthoCameraController.h"
#include "Core/KeyCodes.h"
#include "Events/EventHandler.h"

namespace Pixel {
	constexpr float ZOOM_SPEED = 3.0f;
	OrthoCameraController::OrthoCameraController(glm::vec2& window_size) {
		aspect_ratio = (float) window_size.x / (float) window_size.y;
		camera = OrthoCamera(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	void OrthoCameraController::Update() {
		camera.SetPosition(camera_pos);
		if (EventHandler::KeyPressed(PIXEL_KEY_D))
			camera_pos.x += camera_speed.x;
		if (EventHandler::KeyPressed(PIXEL_KEY_A))
			camera_pos.x -= camera_speed.x;
		if (EventHandler::KeyPressed(PIXEL_KEY_W))
			camera_pos.y += camera_speed.y;
		if (EventHandler::KeyPressed(PIXEL_KEY_S))
			camera_pos.y -= camera_speed.y;
	}

	void OrthoCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		dispatcher.Dispatch<MouseWheelEvent>(PIXEL_BIND_EVENT(MouseWheelHandler));
		dispatcher.Dispatch<ResizeEvent>(PIXEL_BIND_EVENT(WindowResizeHandler));
	}

	bool OrthoCameraController::MouseWheelHandler(MouseWheelEvent& mousewheel) {
		zoom += mousewheel.yoffset * ZOOM_SPEED;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
		return true;
	}

	bool OrthoCameraController::WindowResizeHandler(ResizeEvent& resize) {
		aspect_ratio = (float) resize.width / (float) resize.height;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
		return true;
	}
}