cmake_minimum_required(VERSION 3.16)
project(beast_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.70 REQUIRED COMPONENTS system thread)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

add_executable(regular_beast_server ../regular_beast_server.cpp)
add_executable(regular_beast_client ../regular_beast_client.cpp)
add_executable(concurrent_beast_client ../concurrent_beast_client.cpp)

# 链接 Boost 和 pthread 库
target_link_libraries(regular_beast_client
        PRIVATE
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        Threads::Threads
)

target_link_libraries(regular_beast_server
        PRIVATE
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        Threads::Threads
)

target_link_libraries(concurrent_beast_client
        PRIVATE
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        Threads::Threads
)