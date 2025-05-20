#include <iostream>
#include <atomic>
#include "../../headers/httplib.h"

std::atomic<int> completed_requests(0);
std::atomic<int> failed_requests(0);
std::atomic<long long> total_transfer_time(0);

void perform_request(const std::string& mode, int thread_id)
{
    httplib::Client cli("localhost", 9873);
    std::string path = (mode == "regular") ? "/regular" : "/stream";

    auto start = std::chrono::high_resolution_clock::now();

    std::string received_data;
    auto res = cli.Get(path.c_str(), [&received_data](const char *data, size_t data_length) {
        received_data.append(data, data_length);
        return true;
    });

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (res) {
        completed_requests++;
        total_transfer_time += duration;
        std::cout << "Thread " << thread_id << " - " << mode << " transfer time: " << duration << " ms\n";
    } else {
        failed_requests++;
        std::cerr << "Thread " << thread_id << " - Error: " << res.error() << "\n";
    }
}

int main(int argc, char* argv[])
{
    // httplib::Client cli("localhost", 9873);
    //
    // // 发送 GET 请求
    // auto res = cli.Get("/");
    // if (res && res->status == 200) {
    //     std::cout << res->body << std::endl;
    // }
    //
    // res = cli.Get("/about");
    // if (res && res->status == 200) {
    //     std::cout << res->body << std::endl;
    // }
    //
    // // 发送 POST 请求
    // const httplib::Headers headers = {
    //     // 设置请求头 MIME类型
    //     {"Content.Type", "application/json"}
    // };
    //
    // const std::string body = R"({"name": "Guoding Ji", "age": 23})";
    //
    // if (auto res1 = cli.Post("/api/users", headers, body, "application/json"); res1 && res1->status == 200) {
    //     std::cout << "Request successful: " << res1->body << std::endl;
    // } else {
    //     std::cout << "Request failed: " << std::endl;
    // }

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [regular|stream] [thread_count]\n";
        return 1;
    }

    std::string mode = argv[1];
    int thread_count = std::stoi(argv[2]);

    std::vector<std::thread> threads;
    auto test_start = std::chrono::high_resolution_clock::now();

    // 创建并启动所有线程
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(perform_request, mode, i);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    auto test_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(test_end - test_start).count();

    // 打印汇总统计信息
    std::cout << "\n=== Performance Test Summary ===\n";
    std::cout << "Total threads: " << thread_count << "\n";
    std::cout << "Completed requests: " << completed_requests << "\n";
    std::cout << "Failed requests: " << failed_requests << "\n";
    std::cout << "Total test duration: " << total_duration << " ms\n";
    std::cout << "Average transfer time: "
              << (completed_requests > 0 ? total_transfer_time / completed_requests : 0)
              << " ms\n";
    std::cout << "Requests per second: "
              << (total_duration > 0 ? (completed_requests * 1000.0 / total_duration) : 0)
              << "\n";

    return 0;
}