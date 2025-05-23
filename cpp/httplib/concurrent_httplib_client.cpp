//
// Created by jgd on 25-5-22.
//
#include <iostream>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include "../../headers/httplib.h"

std::atomic<size_t> total_bytes_transferred{0};
std::atomic<size_t> completed_requests{0};
std::atomic<size_t> failed_requests{0};
std::mutex cout_mutex;

void worker_thread(const std::string& host, int port,
                  int requests_per_thread, bool upload,
                  const std::string& large_data) {
    httplib::Client cli(host, port);
    cli.set_connection_timeout(30);
    cli.set_read_timeout(300);

    for (int i = 0; i < requests_per_thread; ++i) {
        bool success = false;
        size_t bytes_transferred = 0;

        try {
            if (upload) {
                auto res = cli.Post("/receive_data", large_data, "text/plain");
                if (res && res->status == 200) {
                    bytes_transferred = large_data.size();
                    success = true;

                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Thread " << std::this_thread::get_id()
                              << " - Upload #" << i+1 << " OK ("
                              << bytes_transferred/(1024 * 1024) << "MB)\n";
                }
            } else {
                std::string received_data;
                auto res = cli.Get("/large_data",
                    [&](const char* data, size_t len) {
                        received_data.append(data, len);
                        return true;
                    });

                if (res && res->status == 200) {
                    bytes_transferred = received_data.size();
                    success = true;

                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Thread " << std::this_thread::get_id()
                              << " - Download #" << i+1 << " OK ("
                              << bytes_transferred/(1024 * 1024) << "MB)\n";
                }
            }
        } catch (const std::exception& e) {
            std::lock_guard lock(cout_mutex);
            std::cerr << "Thread " << std::this_thread::get_id()
                      << " - Error: " << e.what() << "\n";
        }

        if (success) {
            completed_requests++;
            total_bytes_transferred += bytes_transferred;
        } else {
            failed_requests++;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <host> <port> <threads> <requests_per_thread> <upload|download>\n"
                  << "Example: " << argv[0] << " localhost 8848 10 5 upload\n";
        return 1;
    }

    const std::string host = argv[1];
    const int port = std::stoi(argv[2]);
    const int thread_count = std::stoi(argv[3]);
    const int requests_per_thread = std::stoi(argv[4]);
    const bool upload = (std::string(argv[5]) == "upload");

    // 准备测试数据 (30MB)
    const size_t data_size = 30 * 1024;

    const std::string large_data(data_size, 'A');

    std::cout << "Starting test with " << thread_count << " threads, "
              << requests_per_thread << " requests/thread\n"
              << "Mode: " << (upload ? "UPLOAD" : "DOWNLOAD") << "\n";

    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(worker_thread,
                           host, port,
                           requests_per_thread,
                           upload,
                           large_data);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    const double total_mb = total_bytes_transferred / (1024.0 * 1024.0);
    const double mbps = (total_bytes_transferred * 8.0) / (duration * 1000.0);

    std::cout << "\n=== Test Summary ===\n"
              << "Total requests: " << thread_count * requests_per_thread << "\n"
              << "Successful: " << completed_requests << "\n"
              << "Failed: " << failed_requests << "\n"
              << "Total data: " << std::fixed << std::setprecision(2)
              << total_mb << " MB\n"
              << "Total time: " << duration << " ms\n"
              << "Throughput: " << std::setprecision(2)
              << mbps << " Mbps (" << (total_mb / (duration / 1000.0)) << " MB/s)\n"
              << "Req/s: " << (completed_requests * 1000.0 / duration) << "\n";

    return 0;
}