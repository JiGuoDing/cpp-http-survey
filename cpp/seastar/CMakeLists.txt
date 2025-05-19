cmake_minimum_required(VERSION 3.16)
project(seastar_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

find_package(Seastar REQUIRED)

add_executable(seastar_server ../seastar_server.cpp)

target_link_libraries(seastar_server PRIVATE Seastar::seastar)