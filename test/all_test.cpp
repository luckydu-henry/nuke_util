#include <thread>
#include <future>
#include <array>
#include <iostream>
#include <chrono>
#include <nkutil/logging.hpp>
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

    get_logger<lc::console>()->error("Begin!");
    std::thread th(send_log);
    get_logger<lc::console>()->error("End!");
    th.join();

    return 0;
}