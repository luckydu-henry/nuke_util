#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>
#include <string_view>

#include "util/log/LogLogger.hpp"
#include "util/log/LogColor.hpp"

namespace Hunt {
    namespace Util {
        // Global output file.
        static std::ofstream sFstream{gOutputFile.data()};
        // Static instance initialize.
        DbtLogger DbtLogger::scsInst{std::clog, DbtLogger::eConsole};
        DbtLogger DbtLogger::sfsInst{sFstream,  DbtLogger::eFstream};

        DbtLogger::DbtLogger(std::ostream& stream, uint8_t state) : 
        mStream(stream),
        mState(state) {}

        DbtLogger& DbtLogger::Get(DbtLogger::LoggerType lt) {
            switch (lt) {
                case DbtLogger::eConsole:    return DbtLogger::scsInst;
                case DbtLogger::eFstream:    return DbtLogger::sfsInst;
            }
        }
        void DbtLogger::VLog(LogLevel lv, LogColor hexColor, std::string_view fmt, std::format_args args) {
            auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
            auto const id = std::this_thread::get_id();
            // Can't use string_view.
            // string_view will have memory issue.
            std::string shadeUp = (mState == eConsole) ? MakeColorString(eLogForeG, hexColor) : "";
            std::string shadeDn = (mState == eConsole) ? "\033[15;m" : "";

            // Shade up.
            mStream.rdbuf()->sputn(shadeUp.c_str(), shadeUp.size());

            mStream << std::format(gFormatString, time, gLevelString[lv])
                    << "TID: " << std::setw(6) << std::setfill('0') << id << ": "; // std::thread::id not supported by std yet.
            mStream << std::vformat(fmt, args) << std::endl;
            // Shade down
            mStream.rdbuf()->sputn(shadeDn.c_str(), shadeDn.size());
        }
    }
}