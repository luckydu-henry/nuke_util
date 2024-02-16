#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <nkutil/logging.hpp>
#include <nkutil/console_color.hpp>
namespace nkutil {
    std::string default_logger_formatter(const logger_info& info, logger_level lv) {
        auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
        // Until implementation date -- 2024/2/16, gcc & clang & msvc had all implemented thread::id formatter.
        // So I used it here, not bad really, remember to use latest feature level of your compiler.
        // Or you can use C++23 feature level in the future since this is a C++23 feature.
        auto const tid = std::this_thread::get_id();
        // default_logger_formatter uses this format pattern -- TIME THREAD-ID NAME [LEVEL] <space>
        return std::format("{:%Y/%m/%d %X} {:0>10} {:s} [{:s}] ", time, tid, info.logger_name, info.level_string_map[static_cast<size_t>(lv)]);
    }
    std::string make_ansi_console_color(console_color_usage usg, uint32_t cl) {
        uint8_t r = cl >> 16, g = cl >> 8, b = cl;
        return std::format("\033[{:d}8;2;{:d};{:d};{:d}m", (uint8_t)usg, r, g, b);
    }
    std::string make_ansi_console_color() {
        return "\033[15;m"s;
    }
    int init_default_loggers(std::string_view lgname, std::string_view fname) {
        static std::ofstream s_fs(fname.data());
        if (!s_fs.is_open()) {
            return 0;
        }
        logger_info cinfo{ std::cout, lgname };
        static console_logger s_clogger{ cinfo };
        logger_info finfo{ s_fs, lgname };
        static file_logger    s_flogger{ finfo };
        std::get<console_logger*>(g_default_loggers) = &s_clogger;
        std::get<file_logger   *>(g_default_loggers) = &s_flogger;

        return 1;
    }
}


//
//namespace Hunt {
//    namespace Util {
//        // Global output file.
//        static std::ofstream sFstream{gOutputFile.data()};
//        // Static instance initialize.
//        DbtLogger DbtLogger::scsInst{std::clog, DbtLogger::eConsole};
//        DbtLogger DbtLogger::sfsInst{sFstream,  DbtLogger::eFstream};
//
//        DbtLogger::DbtLogger(std::ostream& stream, uint8_t state) : 
//        mStream(stream),
//            mState(state) {
//            mStream.sync_with_stdio(false); // Higher speed when this is off.
//        }
//
//        DbtLogger& DbtLogger::Get(DbtLogger::LoggerType lt) {
//            switch (lt) {
//                case DbtLogger::eConsole:    return DbtLogger::scsInst;
//                case DbtLogger::eFstream:    return DbtLogger::sfsInst;
//            }
//        }
//        void DbtLogger::VLog(LogLevel lv, LogColor hexColor, std::string_view fmt, std::format_args args) {
//            auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
//            // Can't use string_view.
//            // string_view will have memory issue.
//            std::string shadeUp = (mState == eConsole) ? MakeColorString(eLogForeG, hexColor) : "";
//            std::string shadeDn = (mState == eConsole) ? "\033[15;m" : "";
//
//            // std::cout can only work thread safely when output data is only one string.
//            std::string oString = std::format("{:s}{:s}{:s}{:s}\n", 
//                                               shadeUp, std::format(gFormatString, time, gLevelString[lv]), 
//                                               std::vformat(fmt, args), shadeDn);
//
//            // This line is definetly thread safe.
//            mStream.rdbuf()->sputn(oString.data(), oString.size());
//        }
//    }
//}