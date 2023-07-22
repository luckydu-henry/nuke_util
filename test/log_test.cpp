#include <Utlt/log.hpp>
#include <thread>
#include <future>
#include <array>

void send_log() {
    CRS::Utlt::consolelg.log_debug("Hello world!");
    CRS::Utlt::consolelg.log_debug("Hello world!");
    CRS::Utlt::consolelg.log_debug("Hello world!");
    CRS::Utlt::consolelg.log_debug("Hello world!");
    CRS::Utlt::consolelg.log_debug("Hello world!");
}

int main(int argc, char* argv[]) {
    using namespace CRS;
    using color = Utlt::color_t;
    using level = Utlt::level_t;
    try {
        Utlt::consolelg.log_trace("Program begin!");
        auto ret = std::async(std::launch::async, send_log);
        Utlt::consolelg.log_trace("End!");
        Utlt::consolelg.log_trace("End!");
        Utlt::consolelg.log_trace("End!");
        Utlt::consolelg.log_trace("End!");
        Utlt::consolelg.log_trace("End!");
    }
    catch (std::exception& e){
        Utlt::consolelg.log_fatal("{}", e.what());
    }
}