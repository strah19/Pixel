#include "pixelpch.h"
#include "Timer.h"

namespace Pixel {
	namespace Utility {
		Timer::Timer() {
			Start();
		}

		Timer::~Timer() {
			Stop();
		}

		void Timer::Start() {
			start_time = std::chrono::system_clock::now();
			running = true;
		}

		void Timer::Stop() {
			PIXEL_LOG("TIME::%f", ElapsedSeconds());

			end_time = std::chrono::system_clock::now();
			running = false;
		}

		float Timer::ElapsedMilliSeconds() {
			std::chrono::time_point<std::chrono::system_clock> end;

			if (running)
				end = std::chrono::system_clock::now();
			else
				end = end_time;

			return std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();
		}

		float Timer::ElapsedSeconds() {
			return ElapsedMilliSeconds() / 1000.f;
		}
	}
}