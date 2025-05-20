//
// Created by jgd on 25-5-20.
//
// stream_http_server.cc
#include "../../headers/httplib.h"
#include <chrono>
#include <iostream>

constexpr size_t data_size = 30 * 1024 * 1024; // 30MB
constexpr size_t chunk_size = 64 * 1024; // 64KB

int main() {
    httplib::Server svr;

    // 流数据传输端点
    svr.Get("/stream", [](const httplib::Request &, httplib::Response &res) {
        auto start = std::chrono::high_resolution_clock::now();

        res.set_chunked_content_provider(
            "text/plain",
            [](size_t offset, httplib::DataSink &sink) {
                static size_t sent = 0;
                size_t to_send = std::min(chunk_size, data_size - sent);

                if (to_send == 0) {
                    sink.done();
                    return true;
                }

                // 模拟数据块 - 实际应用中可以是文件读取或其他数据源
                std::string chunk(to_send, 'a');
                sink.write(chunk.data(), chunk.size());
                sent += to_send;

                return true;
            });

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Stream transfer server time: " << duration << " ms" << std::endl;
    });

    std::cout << "Stream HTTP server running on http://localhost:9873\n";
    svr.listen("0.0.0.0", 9873);
}