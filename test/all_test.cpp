#include <thread>
#include <future>
#include <array>
#include <iostream>
#include <chrono>
#include <execution>
#include <nkutil/logging.hpp>

#include "nkutil/timer.hpp"
using namespace nkutil;
using lc = logger_category;

void send_log() {
    get_logger<lc::console>()->trace("{:s}", "Hello world!");
    get_logger<lc::console>()->trace("{:s}", "Hello world!");
    get_logger<lc::console>()->trace("{:s}", "Hello world!");
    get_logger<lc::console>()->trace("{:s}", "Hello world!");
}

int main(int argc, char* argv[]) {
    init_default_loggers("BOAR", "output.log");

    auto_wall_timer wtimer{ std::cout };

    return 0;
}