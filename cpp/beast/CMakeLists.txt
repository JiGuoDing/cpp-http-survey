cmake_minimum_required(VERSION 3.16)
project(beast_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

add_executable(regular_beast_server ../regular_beast_server.cpp)
add_executable(regular_beast_client ../regular_beast_client.cpp)

#target_link_libraries(regular_beast_server)
#target_link_libraries(regular_beast_client)
