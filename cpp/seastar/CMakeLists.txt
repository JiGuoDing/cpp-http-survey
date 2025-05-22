cmake_minimum_required(VERSION 3.16)
project(seastar_demo)

# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

find_package(Seastar REQUIRED)

add_executable(regular_seastar_server ../regular_seastar_server.cpp)
add_executable(regular_seastar_client ../regular_seastar_client.cpp)

target_link_libraries(regular_seastar_server PRIVATE Seastar::seastar)
target_link_libraries(regular_seastar_client PRIVATE Seastar::seastar)
