cmake_minimum_required(VERSION 3.16)
project(httplib_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

# 添加可执行文件
add_executable(regular_httplib_server ../regular_httplib_server.cpp)
add_executable(stream_httplib_server ../stream_httplib_server.cpp)
add_executable(httplib_client ../httplib_client.cpp)

# 安装目标到输出目录
install(TARGETS regular_httplib_server stream_httplib_server httplib_client
    RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)