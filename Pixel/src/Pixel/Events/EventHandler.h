#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Core/KeyCodes.h"
#include <glm.hpp>
namespace Pixel {
	class EventHandler {
	public:
		static bool KeyPressed(int keycode);
		static glm::vec2 GetMouseCoordinates();
	};
}

#endif // !EVENT_HANDLER_H
