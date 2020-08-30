#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct KeyboardEvents : public Event {
		KeyboardEvents(const std::string& name)
			: Event(name), name(), pressed(false), repeat(0) { }
		virtual ~KeyboardEvents() = default;

		std::string GetName() const { return name; }

		bool pressed;
		int repeat;
		std::string name;
	};
}

#endif // !KEYBOARD_EVENTS_H
