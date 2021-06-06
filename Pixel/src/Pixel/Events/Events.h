#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <functional>

#define PIXEL_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pixel {
	class Event {
	public:
		Event(const std::string& name)
			: name(name) { }
		virtual ~Event() = default;

		virtual std::string GetName() const = 0;
	protected:
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
		bool Dispatch(const std::function<void(T&)> func) {
			if (dynamic_cast<T*>(event)) {
				func(static_cast<T&>(*event));
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
