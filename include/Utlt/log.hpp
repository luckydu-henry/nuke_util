/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include <format>
#include <ostream>

#define LOG_VERSION "0.1.0"

namespace CRS {
    namespace Utlt {
        static const char* g_log_file_path = "outlog.log";

        enum class level_t { trace, debug, info, warn, error, fatal };
        enum class color_t { black, red, green, yellow, blue, mangenta, cyan, white };

        inline constexpr std::string_view g_level_name[6] = { 
            "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" 
        };
        // Default color to use console output.
        inline constexpr char default_fg_color = static_cast<char>(color_t::white);
        inline constexpr char default_bg_color = static_cast<char>(color_t::black);
        // Format description: [Time Level Tid] 
        inline constexpr std::string_view g_fmt_string = "[{} {:s}] ";
        // This stores fg and bg color together. Which can sets and gets colors.
        struct color_pack {
            color_t fg, bg;
            // This one is platform specific for Win32 it's SetConsoleTextAttribute.
            // For *nix and MacOSX it just can be ASCII escape code 
            void shade_up();
            // This sets color to default color fg = white and bg = black.
            void shade_dn();
        };
        // Biostance (Binary-Instance) logger. One is console logger and one is fstream logger.
        class dbt_logger {
            // Disable constructor and destructor.
            // lg should be one of console or file.
             dbt_logger(std::ostream& stream);
            ~dbt_logger() = default;

            static dbt_logger  s_csinst, s_fsinst;
            static std::mutex  s_mutex;
            std::ostream&      m_stream;
        public:
            // Use this to get desired logger one is console and one is file
            typedef enum { console, file } logger_t;

            static dbt_logger& get(logger_t lg);

            void   vlog(level_t lv, color_pack cl, std::string_view fmt, std::format_args args);

            template <typename ... Args>
            inline void log(level_t lv, color_pack cl, std::string_view fmt, const Args&...args) {
                vlog(lv, cl, fmt, std::make_format_args(args...));
            }
            template <typename ... Args>
            inline void log_trace(std::string_view fmt, const Args& ... args) {
                log(level_t::trace, { color_t::white, color_t::black }, fmt, args...);
            }
            template <typename ... Args>
            inline void log_debug(std::string_view fmt, const Args& ... args) {
                log(level_t::debug, { color_t::cyan, color_t::black }, fmt, args...);
            }
            template <typename ... Args>
            inline void log_info(std::string_view fmt, const Args& ... args) {
                log(level_t::info, { color_t::green, color_t::black }, fmt, args...);
            }
            template <typename ... Args>
            inline void log_warn(std::string_view fmt, const Args& ... args) {
                log(level_t::warn, { color_t::yellow, color_t::black }, fmt, args...);
            }
            template <typename ... Args>
            inline void log_error(std::string_view fmt, const Args& ... args) {
                log(level_t::error, { color_t::mangenta, color_t::black }, fmt, args...);
            }
            template <typename ... Args>
            inline void log_fatal(std::string_view fmt, const Args& ... args) {
                log(level_t::fatal, { color_t::red, color_t::black }, fmt, args...);
            }
        };

        inline dbt_logger& consolelg = dbt_logger::get(dbt_logger::console);
        inline dbt_logger& fstreamlg = dbt_logger::get(dbt_logger::file);
    }
}