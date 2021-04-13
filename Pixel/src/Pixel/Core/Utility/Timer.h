#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include "Core/Logger.h"

namespace Pixel {
	namespace Utility {
		class Timer {
		public:
			Timer();
			~Timer();

			void Start();
			void Stop();
			float ElapsedMilliSeconds();
			float ElapsedSeconds();
		private:
			std::chrono::time_point<std::chrono::system_clock> start_time;
			std::chrono::time_point<std::chrono::system_clock> end_time;
			bool running = false;
		};

#define PIXEL_PROFILE PIXEL_LOG_GOOD("DEBUG_TIMER_STARTED::%s::LINE::%d::TIME::%s", __FUNCTION__, __LINE__, __TIME__) Pixel::Utility::Timer t 
	}
}

#endif // !TIMER_H
