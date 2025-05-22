cmake_minimum_required(VERSION 3.31)
project(http)

set(CMAKE_CXX_STANDARD 20)

add_executable(http cpp/main.cpp
        cpp/drogon/stream_drogon_server.cpp
        cpp/drogon/stream_drogon_client.cpp
        cpp/httplib/concurrent_httplib_client.cpp
        cpp/httplib/concurrent_httplib_server.cpp
        cpp/beast/regular_beast_server.cpp
        cpp/beast/regular_beast_client.cpp)
