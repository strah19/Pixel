#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(const std::string& name)
			: Event(name), x(0), y(0), down(false), button_id(0), clicks(0) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		int x, y;
		bool down;
		int button_id;
		int clicks;
	};
}

#endif // !MOUSE_EVENTS_H
