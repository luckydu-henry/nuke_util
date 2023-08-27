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
            mState(state) {
            mStream.sync_with_stdio(false); // Higher speed when this is off.
        }

        DbtLogger& DbtLogger::Get(DbtLogger::LoggerType lt) {
            switch (lt) {
                case DbtLogger::eConsole:    return DbtLogger::scsInst;
                case DbtLogger::eFstream:    return DbtLogger::sfsInst;
            }
        }
        void DbtLogger::VLog(LogLevel lv, LogColor hexColor, std::string_view fmt, std::format_args args) {
            auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
            // Can't use string_view.
            // string_view will have memory issue.
            std::string shadeUp = (mState == eConsole) ? MakeColorString(eLogForeG, hexColor) : "";
            std::string shadeDn = (mState == eConsole) ? "\033[15;m" : "";

            // std::cout can only work thread safely when output data is only one string.
            std::string oString = std::format("{:s}{:s}{:s}{:s}\n", 
                                               shadeUp, std::format(gFormatString, time, gLevelString[lv]), 
                                               std::vformat(fmt, args), shadeDn);

            // This line is definetly thread safe.
            mStream.rdbuf()->sputn(oString.data(), oString.size());
        }
    }
}