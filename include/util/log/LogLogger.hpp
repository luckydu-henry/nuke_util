#pragma once
#include <stdint.h>

#include <format>
#include <syncstream>

#include "LogColor.hpp"

namespace Hunt {

    namespace Util {

        enum LogLevel { eTrace = 0, eDebug, eInfo, eWarn, eError, eFatal };
        // Format description [Time Level TID]
        inline constexpr std::string_view gFormatString = "[{:%Y-%m-%d %X} Hunt:{:5s}] ";
        // These are bind with LogLevel indecies.
        inline constexpr std::string_view gLevelString[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
        // Default output log file
        inline constexpr std::string_view gOutputFile = "OutputLog.log";

        // Dbt (Double-Instance) logger. One is console logger and one is fstream logger.
        class DbtLogger {
            // Disable constructor and destructor.
            // lg should be one of console or file.
            DbtLogger(std::ostream& stream, uint8_t state);
            ~DbtLogger() = default;

            static DbtLogger    scsInst, sfsInst;
            std::ostream&       mStream;
            uint8_t             mState;  // Fstream doesn't support.
        public:
            // Use this to get desired logger one is console and one is file
            enum LoggerType : uint8_t { eConsole, eFstream };

            static DbtLogger& Get(LoggerType lg);

            void   VLog(LogLevel lv, LogColor hexColor, std::string_view fmt, std::format_args args);

            template <typename ... Args>
            void   Log(LogLevel lv, LogColor hexColor, std::string_view fmt, const Args&...args) {
                VLog(lv, hexColor, fmt, std::make_format_args(args...));
            }

            template <typename ... Args>
            void   Trace(std::string_view fmt, const Args& ... args) {
                Log(eTrace, eLogColorWhite, fmt, args...);
            }
            template <typename ... Args>
            void   Debug(std::string_view fmt, const Args& ... args) {
                Log(eDebug, eLogColorCyan, fmt, args...);
            }
            template <typename ... Args>
            void   Info(std::string_view fmt, const Args& ... args) {
                Log(eInfo, eLogColorGreen, fmt, args...);
            }
            template <typename ... Args>
            void   Warn(std::string_view fmt, const Args& ... args) {
                Log(eWarn, eLogColorYellow, fmt, args...);
            }
            template <typename ... Args>
            void   Error(std::string_view fmt, const Args& ... args) {
                Log(eError, eLogColorMagenta, fmt, args...);
            }
            template <typename ... Args>
            void   Fatal(std::string_view fmt, const Args& ... args) {
                Log(eFatal, eLogColorRed, fmt, args...);
            }
        };

        inline DbtLogger& lConsole = DbtLogger::Get(DbtLogger::eConsole);
        inline DbtLogger& lFstream = DbtLogger::Get(DbtLogger::eFstream);
    }
}