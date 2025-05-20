//
// Created by jgd on 25-5-20.
//
#include <iostream>
#include <atomic>
#include "../../headers/httplib.h"

constexpr size_t data_size = 30 * 1024 * 1024;
std::atomic<int> connection_count(0);

int main()
{
    httplib::Server svr;

    // 准备30MB数据
    const std::string large_data(data_size, 'a');

    // 普通数据传输端点
    svr.Get("/regular", [&](const httplib::Request&, httplib::Response& res)
    {
        connection_count++;
        auto start = std::chrono::high_resolution_clock::now();
        res.set_content(large_data, "text/plain");

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Connection#" << connection_count << " - Regular transfer server time: " << duration << "ms" << std::endl;
    });

    // 添加监控端点
    svr.Get("/stats", [&](const httplib::Request&, httplib::Response& res)
    {
        res.set_content("Total connections: " + std::to_string(connection_count), "text/plain");
    });

        std::cout << "Regular HTTP server running on http://localhost:9873\n";
        svr.listen("0.0.0.0", 9873);

    return 0;
}
