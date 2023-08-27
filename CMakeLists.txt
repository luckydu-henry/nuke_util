cmake_minimum_required(VERSION 3.15)
project(Hunt_util LANGUAGES C CXX VERSION 1.0)

set(HUNT_UTIL_BUILD_TEST ON)

file(GLOB LOG_HEADER   "include/util/log/*.hpp" "include/util/log.hpp")
file(GLOB LOG_SOURCES  "src/log/*.cpp")
# Set global include directory
set(INC_PATH "include/")
# Add source groups
source_group(log FILES ${LOG_HEADER} ${LOG_SOURCES})
# Add static link library
add_library               (Hunt_util_lib STATIC ${LOG_HEADER} ${LOG_SOURCES})
target_compile_features   (Hunt_util_lib PUBLIC cxx_std_20)
target_include_directories(Hunt_util_lib PUBLIC ${INC_PATH})

if (HUNT_UTIL_TEST_ENABLE)
    add_executable            (Hunt_util_log_test "test/LogTest.cpp")
    target_include_directories(Hunt_util_log_test PUBLIC ${INC_PATH})
    target_compile_features   (Hunt_util_log_test PUBLIC cxx_std_20)
    target_link_libraries     (Hunt_util_log_test PUBLIC Hunt_util_lib)
endif(HUNT_UTIL_TEST_ENABLE)