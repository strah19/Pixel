#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct QuitEvent : public Event {
		QuitEvent(const std::string& name)
			: Event(name), closed(false) { }
		virtual ~QuitEvent() = default;

		std::string GetName() const { return name; }

		bool closed;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(const std::string& name)
			: Event(name), resize(false) { }
		virtual ~ResizeEvent() = default;

		std::string GetName() const { return name; }

		bool resize;
	};
}

#endif // !WINDOW_EVENTS_H
