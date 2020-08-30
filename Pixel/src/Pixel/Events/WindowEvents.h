#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Events/Events.h"

namespace Pixel {
	struct QuitEvent : public Event {
		QuitEvent()
			: Event("QuitEvent") { }
		virtual ~QuitEvent() = default;

		std::string GetName() const { return name; }
	};

	struct ResizeEvent : public Event {
		ResizeEvent(int width, int height)
			: Event("ResizeEvent"), width(width), height(height) { }
		virtual ~ResizeEvent() = default;

		std::string GetName() const { return name; }

		int width, height;
	};
}

#endif // !WINDOW_EVENTS_H
