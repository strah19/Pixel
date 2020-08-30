#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <functional>

#define PIXEL_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pixel {
	using EventType = std::size_t;

	inline EventType GetEventType() {
		static EventType current__type = 0;
		return current__type++;
	}

	template <typename T> inline EventType GetEventType() {
		static EventType type = GetEventType();
		return type;
	}

	class Event {
	public:
		Event(const std::string& name)
			: active(true), Handled(false), name(name) { }
		virtual ~Event() = default;

		bool ActivityCheck() const { return active; }
		void Active(bool active) { this->active = active; }

		bool Handled;

		virtual std::string GetName() const = 0;
	protected:
		bool active;
		std::string name;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event* event)
			: event(event) {
		}

		virtual ~EventDispatcher() {
			event = nullptr;
		}

		template<typename T>
		bool Dispatch(const std::function<bool(T&)> func) {
			if (event->ActivityCheck() && dynamic_cast<T*>(event)) {
				event->Handled = func(static_cast<T&>(*event));
				return true;
			}
			return false;
		}

		inline Event* GetEvent() { return event; }
	private:
		Event* event;
	};
}

#endif // !EVENTS_H
