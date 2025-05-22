cmake_minimum_required(VERSION 3.16)
project(httplib_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

# 查找pthread库
find_package(Threads REQUIRED)

# 添加可执行文件
add_executable(regular_httplib_server ../regular_httplib_server.cpp)
#add_executable(stream_httplib_server ../stream_httplib_server.cpp)
add_executable(regular_httplib_client ../regular_httplib_client.cpp)
add_executable(concurrent_httplib_server ../concurrent_httplib_server.cpp)
add_executable(concurrent_httplib_client ../concurrent_httplib_client.cpp)

# 链接 pthread 库到所有目标
target_link_libraries(regular_httplib_client PRIVATE Threads::Threads)
target_link_libraries(regular_httplib_server PRIVATE Threads::Threads)
target_link_libraries(concurrent_httplib_server PRIVATE Threads::Threads)
target_link_libraries(concurrent_httplib_client PRIVATE Threads::Threads)
#target_link_libraries(stream_httplib_server PRIVATE Threads::Threads)

# 安装目标到输出目录
install(TARGETS regular_httplib_server regular_httplib_client concurrent_httplib_server concurrent_httplib_client
    RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)