#pragma once
#include <ratio>
#include <string>
#include <functional>
#include <ostream>

namespace nkutil {
    
    /// @enum  timer_unit
    /// @brief Adjust what units to use.
    enum class timer_unit { hour, minute, second, millisecond, microsecond, nanosecond };

    template <timer_unit U>
    struct timer_unit_traits;

    template <>
    struct timer_unit_traits<timer_unit::hour> {
        using ratio                           = std::ratio<3600000>;
    };
    template <>
    struct timer_unit_traits<timer_unit::minute> {
        using ratio                           = std::ratio<60000>;
    };
    template <>
    struct timer_unit_traits<timer_unit::second> {
        using ratio                           = std::kilo;
    };
    template <>
    struct timer_unit_traits<timer_unit::millisecond> {
        using ratio                           = std::ratio<1>;
    };
    template <>
    struct timer_unit_traits<timer_unit::microsecond> {
        using ratio                           = std::milli;
    };
    template <>
    struct timer_unit_traits<timer_unit::nanosecond> {
        using ratio                           = std::micro;
    };

    /// @enum  timer_category
    /// @brief Wall calculate difference of begin time and end time; Cpu calculate tick times.
    enum class timer_category { wall, cpu };
    // This alias is for custom time output. long double can basically handle all kinds of forms. 
    using timer_formatter_t = std::function<std::string(long double, timer_category, timer_unit)>;

    std::string default_timer_formatter(long double t, timer_category c, timer_unit u);

    namespace detail {
        // With impl wrapper we can hide all details to the cpp file.
        template <timer_category Category, typename Ty>
        struct impl_timer {
            static void              tick(Ty& obj);
        };
    }
    ///
    /// @class   basic_timer
    /// @brief   Template prototype of different timers.
    /// @details ~
    /// @tparam  Ty - result of 'count' in what form -- integer or floating point.
    /// @tparam  Category - Wall / Cpu
    /// @tparam  IsAuto   - Auto tick and auto format when destruct.
    ///
    template <typename Ty, timer_category Category, bool IsAuto>
    class basic_timer {
        Ty                start_, stop_;
        timer_formatter_t formatter_;
        std::ostream*     os_;
    public:
        using                 value_type     = Ty;
        static constexpr auto default_unit   = timer_unit::millisecond;
        static constexpr auto category       = Category;
        static constexpr auto is_auto_timer = IsAuto;

        // Must give a proper ostream to auto timers.
        basic_timer(std::ostream& os, timer_formatter_t fmtter = default_timer_formatter) : formatter_(fmtter) {
            if constexpr (is_auto_timer) {
                os_ = nullptr;
                detail::impl_timer<category, value_type>::tick(start_);
            }
            else {
                start_ = static_cast<value_type>(0);
            }
            os_   = &os;
            stop_ = 0;
        }
        basic_timer() : start_(0), stop_(0), formatter_(default_timer_formatter), os_(nullptr) {}

        basic_timer(const basic_timer&) = delete;
        basic_timer(basic_timer&&)      = delete;
        basic_timer& operator=(const basic_timer&) = delete;
        basic_timer& operator=(basic_timer&&)      = delete;

        void        start() {
            detail::impl_timer<category, value_type>::tick(start_);
        }
        void        stop() {
            detail::impl_timer<category, value_type>::tick(stop_);
        }
        // Return value < 0 means there's an error occured.
        // Pass in a std::ratio class to change time unit.
        template <timer_unit U = default_unit>
        value_type  elapsed() const {
            using ratio = typename timer_unit_traits<U>::ratio;
            // Since timers uses milliseconds by default.
            auto const diff = ((stop_ - start_) * static_cast<value_type>(ratio::den)) / static_cast<value_type>(ratio::num);
            return diff;
        }
        template <timer_unit U = default_unit>
        std::string format() const {
            return formatter_(static_cast<long double>(elapsed<U>()), category, U);
        }
        ~basic_timer() {
            if constexpr (is_auto_timer) {
                detail::impl_timer<category, value_type>::tick(stop_);
                auto const str = format();
                os_->rdbuf()->sputn(str.data(), str.size());
                os_->put('\n');
            }
        }
    };
    // Aliases makes our life easier. 
    using wall_timer = basic_timer<long double, timer_category::wall, false>;
    using cpu_timer  = basic_timer<long double, timer_category::cpu,  false>;

    using auto_wall_timer = basic_timer<long double, timer_category::wall, true>;
    using auto_cpu_timer  = basic_timer<long double, timer_category::cpu,  true>;
}
