#include <thread>
#include <future>
#include <array>

#include <util/log/LogLogger.hpp>
#include <util/log/LogColor.hpp>

using namespace Hunt::Util;

void SendLog() {
    lConsole.Trace("Hello world!");
    lConsole.Warn("Hello world!");
    lConsole.Error("Hello world!");
    lConsole.Info("Hello world!");
    lConsole.Debug("Hello world!");
}

int main(int argc, char* argv[]) {

    lConsole.Fatal("Start!");
    std::thread th(SendLog);
    th.join();
    lConsole.Fatal("End!");


    return 0;
}