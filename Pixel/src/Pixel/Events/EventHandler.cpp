#include "pixelpch.h"
#include "EventHandler.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Core/Application.h"

namespace Pixel {
	bool EventHandler::KeyPressed(int keycode) {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	glm::vec2 EventHandler::GetMouseCoordinates() {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}
}