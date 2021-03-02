#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool action, int button_id)
			: Event("Mouse Button Event"), action(action), button_id(button_id) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		int action;
		int button_id;
	};

	struct MousePositionEvent : public Event {
		MousePositionEvent(float x, float y)
			: Event("Mouse Position Event"), x(x), y(y) { }
		virtual ~MousePositionEvent() = default;

		std::string GetName() const { return name; }

		float x, y;
	};

	struct MouseWheelEvent : public Event {
		MouseWheelEvent(float xoffset, float yoffset) : Event("Mouse Wheel"), xoffset(xoffset), yoffset(yoffset) { }
		virtual ~MouseWheelEvent() = default;

		std::string GetName() const { return name; }

		float xoffset, yoffset;
	};
}

#endif // !MOUSE_EVENTS_H
