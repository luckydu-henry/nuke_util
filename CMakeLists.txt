cmake_minimum_required(VERSION 3.15)
project(CRS_Utlt LANGUAGES C CXX VERSION 1.0)

file(GLOB_RECURSE SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

set(INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library               (CRS_Utlt_lib STATIC ${SRC_LIST} ${HEADER_LIST})
target_compile_features   (CRS_Utlt_lib PUBLIC cxx_std_20)
target_include_directories(CRS_Utlt_lib PUBLIC ${INC_PATH})

add_executable            (CRS_Utlt_log_test "${CMAKE_CURRENT_SOURCE_DIR}/test/log_test.cpp")
target_include_directories(CRS_Utlt_log_test PUBLIC ${INC_PATH})
target_compile_features   (CRS_Utlt_log_test PUBLIC cxx_std_20)
target_link_libraries     (CRS_Utlt_log_test PUBLIC CRS_Utlt_lib)