cmake_minimum_required(VERSION 3.16)
project(drogon_demo)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../executable)

# 查找依赖库
find_package(Drogon REQUIRED)
find_package(Jsoncpp REQUIRED)

# 添加可执行文件
add_executable(regular_drogon_server ../regular_drogon_server.cpp)
add_executable(regular_drogon_client ../regular_drogon_client.cpp)
add_executable(concurrent_drogon_client ../concurrent_drogon_client.cpp)

# 链接库
target_link_libraries(regular_drogon_server
    PRIVATE
    Drogon::Drogon
    jsoncpp
)

target_link_libraries(regular_drogon_client
    PRIVATE
    Drogon::Drogon
    jsoncpp
)

target_link_libraries(concurrent_drogon_client
        PRIVATE
        Drogon::Drogon
        jsoncpp
)

# 安装目标到输出目录
install(TARGETS regular_drogon_server regular_drogon_client concurrent_drogon_client
    RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)