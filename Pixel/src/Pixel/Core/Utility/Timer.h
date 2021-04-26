#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <atomic>

namespace Pixel {
	namespace Utility {
        template <typename Clock = std::chrono::high_resolution_clock>
        class Timer
        {
            const typename Clock::time_point start_point;
        public:
            Timer() :
                start_point(Clock::now())
            {}

            template <typename Rep = typename Clock::duration::rep, typename Unit = std::chrono::microseconds>
            Rep ElapsedTime() const
            {
                std::atomic_thread_fence(std::memory_order_relaxed);
                auto counted_time = std::chrono::duration_cast<Unit>(Clock::now() - start_point).count();
                std::atomic_thread_fence(std::memory_order_relaxed);
                return static_cast<Rep>(counted_time);
            }
        };

        using PreciseTimer = Timer<>;
        using SystemTimer = Timer<std::chrono::system_clock>;
        using MonotonicTimer = Timer<std::chrono::steady_clock>;
	}
}

#endif // !TIMER_H
