cmake_minimum_required(VERSION 3.15)
project(nuke_util LANGUAGES C CXX VERSION 1.0)

file(GLOB HEADERS   "include/nkutil/*.hpp")
file(GLOB SOURCES   "src/*.cpp")
# Set global include directory
set(INC_PATH "include/")
# Add static link library
add_library               (nkutil_lib STATIC ${HEADERS} ${SOURCES})
target_compile_features   (nkutil_lib PUBLIC cxx_std_23)
target_include_directories(nkutil_lib PUBLIC ${INC_PATH})

add_executable            (nkutil_test "test/all_test.cpp")
target_include_directories(nkutil_test PUBLIC ${INC_PATH})
target_compile_features   (nkutil_test PUBLIC cxx_std_23)
target_link_libraries     (nkutil_test PUBLIC nkutil_lib)