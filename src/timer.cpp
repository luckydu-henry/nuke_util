#include <format>
// My timer implementation is platform specific.
#ifdef _WIN32
#  include <windows.h>
#else // Unix
#  include <time.h>
#endif

#include "nkutil/timer.hpp"

#include <chrono>
#include <iostream>

namespace nkutil {
    std::string default_timer_formatter(long double t, timer_category c, timer_unit u) {
        using namespace std::string_view_literals;
        std::string_view category_mark = (c == timer_category::cpu) ? "CPU"sv : "Wall"sv;
        std::string_view unit_mark = 
            (u == timer_unit::hour)        ? "h"sv :
            (u == timer_unit::minute)      ? "min"sv :
            (u == timer_unit::second)      ? "s"sv :
            (u == timer_unit::millisecond) ? "ms"sv :
            (u == timer_unit::microsecond) ? "us"sv : "ns";
        return std::format("{:s} time elapsed: {:f} {:3s}", category_mark, t, unit_mark);
    }
}

namespace nkutil::detail {
    // Can only support these types.
    template struct impl_timer<timer_category::cpu, double>;
    template struct impl_timer<timer_category::wall, double>;
    template struct impl_timer<timer_category::cpu, long double>;
    template struct impl_timer<timer_category::wall, long double>;
    template struct impl_timer<timer_category::cpu,  long long>;
    template struct impl_timer<timer_category::wall, long long>;

    template <timer_category Category, typename Ty>
    void impl_timer<Category, Ty>::tick(Ty& obj) {

        if constexpr (Category == timer_category::wall) {
            auto const t = std::chrono::high_resolution_clock::now();
            auto const d = std::chrono::duration_cast<std::chrono::duration<Ty, std::milli>>(t.time_since_epoch());
            obj = d.count();
        }
        else {
#ifdef _WIN32
            FILETIME a, b, c, d;
            if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
                const uint64_t total = (static_cast<uint64_t>(d.dwHighDateTime << 32) | d.dwLowDateTime);
                obj = static_cast<long double>(total) * 1e-4;
            }
            else {
                obj = -1;
            }
#else
            struct timespec tspc
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tspc);
            obj = static_cast<Ty>((long double)tspc.tv_nsec * 1e-6);
#endif
        }
    }
}
