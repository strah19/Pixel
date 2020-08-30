#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct KeyboardEvents : public Event {
		KeyboardEvents(int key, int scancode, int action)
			: Event("Keyboard Event"), key(key), scancode(scancode), action(action) { }
		virtual ~KeyboardEvents() = default;

		std::string GetName() const { return name; }

		int key;
		int scancode;
		int action;
	};
}

#endif // !KEYBOARD_EVENTS_H
