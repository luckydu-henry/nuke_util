#pragma once
#include <array>
#include <format>
#include <memory>
#include <string_view>
#include <cstdint>
#include <functional>

#include "console_color.hpp"

namespace nkutil {
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    // Levels are static -- only contains these levels.
    // However, we can add more levels to the enum class.
    enum class logger_level { trace = 0, info, warn, error, fatal, _level_count };
    // Now only contains file and console two category.
    enum class logger_category { file, console };

    inline constexpr auto g_logger_level_count = static_cast<std::size_t>(logger_level::_level_count);
    struct logger_info;
    // For custom format string.
    using  logger_formatter_t = std::function<std::string(const logger_info&, logger_level)>;
    // This would be assigned to formatter.
    std::string default_logger_formatter(const logger_info&, logger_level);

    struct logger_info {
        // Must fill in stream object, however logger_name can actually ignore.
        std::ostream&    object;
        std::string_view logger_name;
        // Default uses ascii characters as level flag, but you can change them to string.
        std::array<std::string_view, g_logger_level_count> level_string_map = {"~"sv, "="sv, "^"sv, "*"sv, "!"sv};
        // Logger name and level string will be provided by info but time format must be handled by users.
        logger_formatter_t formatter = default_logger_formatter;
    };
    // The template prototype of logger class.
    // File logger use this is enough console logger supports color output.
    template <logger_category Category>
    class basic_logger {
        logger_info      info_;
    public:
        static constexpr auto category = Category;

        constexpr basic_logger(const logger_info& info) : info_(info) {}
        // Logger object can not be copied.
        constexpr basic_logger(const basic_logger&)            = delete;
        constexpr basic_logger(basic_logger&&)                 = default;
        constexpr basic_logger& operator=(const basic_logger&) = delete;
        constexpr basic_logger& operator=(basic_logger&&)      = default;

        void vlog(hex_colors cl, logger_level lv, std::string_view fmt, std::format_args args) {
            if constexpr (category == logger_category::console) {
                auto sup = make_ansi_console_color(console_fg, cl);
                info_.object.rdbuf()->sputn(sup.data(), sup.size());
            }
            // Log these out.
            auto stamp = info_.formatter(info_, lv);
            auto linfo = std::vformat(fmt, args);
            info_.object.rdbuf()->sputn(stamp.data(), stamp.size());
            info_.object.rdbuf()->sputn(linfo.data(), linfo.size());
            // Like std::endl but not flushed.
            info_.object.put('\n');
            // RAII like color down.
            if constexpr (category == logger_category::console) {
                auto sdn = make_ansi_console_color();
                info_.object.rdbuf()->sputn(sdn.data(), sdn.size());
            }
        }

        template <typename ... Args>
        void log(hex_colors cl, logger_level lv, std::string_view fmt, const Args& ... args) {
            vlog(cl, lv, fmt, std::make_format_args(args...));
        }

        template <typename ... Args>
        void trace(std::string_view fmt, const Args& ... args) {
            log(hex_white, logger_level::trace, fmt, args...);
        }
        template <typename ... Args>
        void info(std::string_view fmt, const Args& ... args) {
            log(hex_cyan, logger_level::info, fmt, args...);
        }
        template <typename ... Args>
        void warn(std::string_view fmt, const Args& ... args) {
            log(hex_orange, logger_level::warn, fmt, args...);
        }
        template <typename ... Args>
        void error(std::string_view fmt, const Args& ... args) {
            log(hex_pink, logger_level::error, fmt, args...);
        }
        template <typename ... Args>
        void fatal(std::string_view fmt, const Args& ... args) {
            log(hex_magenta, logger_level::fatal, fmt, args...);
        }

        ~basic_logger() = default;
    };
    // Alias for uses to create
    using console_logger = basic_logger<logger_category::console>;
    using file_logger    = basic_logger<logger_category::file>;
    // Two default loggers uses default logger_info but you can create your own.
    inline std::tuple<console_logger*, file_logger*> g_default_loggers;
    // Use this function to initialize default loggers
    int init_default_loggers(std::string_view lgname, std::string_view fname);

    template <logger_category Category>
    basic_logger<Category>* get_logger() { return std::get<basic_logger<Category>*>(g_default_loggers); }
}