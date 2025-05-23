#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <cstdlib>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

struct Config {
    std::string host;
    unsigned short port;
    size_t data_size_mb;
    int requests_count;
};

struct Stats {
    size_t bytes_transferred = 0;
    std::chrono::milliseconds duration = std::chrono::milliseconds(0);
    size_t requests_sent = 0;
};

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <host> <port> <data_size_mb> <requests_count>\n"
              << "Example: " << program_name << " 127.0.0.1 8080 30 5\n";
}

Config parse_arguments(int argc, char* argv[]) {
    Config config;

    if (argc != 5) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    try {
        config.host = argv[1];
        config.port = static_cast<unsigned short>(std::atoi(argv[2]));
        config.data_size_mb = static_cast<size_t>(std::atoi(argv[3]));
        config.requests_count = std::atoi(argv[4]);

        if (config.port == 0 || config.data_size_mb == 0 || config.requests_count == 0) {
            throw std::invalid_argument("Invalid argument value");
        }
    } catch (...) {
        std::cerr << "Error: Invalid arguments\n";
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    return config;
}

void log_request(const http::request<http::string_body>& req) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << "\n[Client] " << std::put_time(std::localtime(&now_c), "%F %T")
              << " | Sending request:\n"
              << "  Method: " << req.method_string() << "\n"
              << "  Target: " << req.target() << "\n"
              << "  Version: " << req.version() / 10 << "." << req.version() % 10 << "\n"
              << "  Headers:\n";

    for (const auto& field : req) {
        std::cout << "    " << field.name_string() << ": " << field.value() << "\n";
    }
    std::cout << "  Body size: " << req.body().size() << " bytes\n";
}

void log_response(const http::response<http::string_body>& res) {
    std::cout << "[Client] Received response:\n"
              << "  Status: " << res.result_int() << " " << res.reason() << "\n"
              << "  Version: " << res.version() / 10 << "." << res.version() % 10 << "\n"
              << "  Headers:\n";

    for (const auto& field : res) {
        std::cout << "    " << field.name_string() << ": " << field.value() << "\n";
    }
    std::cout << "  Body size: " << res.body().size() << " bytes\n";
}

std::string generate_test_data(size_t size_mb) {
    const size_t size = size_mb * 1024 * 1024;
    std::cout << "[Client] Generating " << size_mb << "MB test data...\n";
    return std::string(size, 'A');
}

Stats send_request(net::io_context& ioc, const Config& config) {
    tcp::resolver resolver(ioc);
    tcp::socket socket(ioc);
    auto endpoints = resolver.resolve(config.host, std::to_string(config.port));

    auto connect_start = std::chrono::steady_clock::now();
    net::connect(socket, endpoints);
    auto connect_end = std::chrono::steady_clock::now();

    std::cout << "[Client] Connected to " << config.host << ":" << config.port
              << " in " << std::chrono::duration_cast<std::chrono::milliseconds>(
                  connect_end - connect_start).count() << "ms\n";

    http::request<http::string_body> req{http::verb::post, "/", 11};
    req.set(http::field::host, config.host);
    req.set(http::field::user_agent, "Beast Client");
    req.set(http::field::content_type, "application/octet-stream");
    req.set(http::field::connection, "close");

    req.body() = generate_test_data(config.data_size_mb);
    req.prepare_payload();

    log_request(req);

    auto write_start = std::chrono::steady_clock::now();
    http::write(socket, req);
    auto write_end = std::chrono::steady_clock::now();

    std::cout << "[Client] Request sent in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                  write_end - write_start).count() << "ms\n";

    beast::flat_buffer buffer;
    http::response<http::string_body> res;

    auto read_start = std::chrono::steady_clock::now();
    http::read(socket, buffer, res);
    auto read_end = std::chrono::steady_clock::now();

    log_response(res);

    std::cout << "[Client] Response received in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                  read_end - read_start).count() << "ms\n";

    Stats stats;
    stats.bytes_transferred = req.body().size();
    stats.duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        read_end - write_start);
    stats.requests_sent = 1;

    return stats;
}

int main(int argc, char* argv[]) {
    try {
        Config config = parse_arguments(argc, argv);

        net::io_context ioc;
        Stats total_stats;
        auto client_start = std::chrono::steady_clock::now();

        std::cout << "[Client] Starting benchmark to " << config.host << ":" << config.port
                  << " with " << config.data_size_mb << "MB per request, "
                  << config.requests_count << " requests\n";

        for (int i = 0; i < config.requests_count; ++i) {
            try {
                std::cout << "\n[Client] ===== REQUEST #" << i + 1 << " =====\n";
                Stats stats = send_request(ioc, config);
                total_stats.bytes_transferred += stats.bytes_transferred;
                total_stats.duration += stats.duration;
                total_stats.requests_sent += stats.requests_sent;

                double throughput = (stats.bytes_transferred / (1024.0 * 1024.0)) /
                                  (stats.duration.count() / 1000.0);

                std::cout << "[Client] Request #" << i + 1 << " stats:\n"
                          << "  Bytes: " << stats.bytes_transferred << "\n"
                          << "  Roundtrip time: " << stats.duration.count() << "ms\n"
                          << "  Throughput: " << std::fixed << std::setprecision(2)
                          << throughput << " MB/s\n";
            } catch (const std::exception& e) {
                std::cerr << "[Client] Error in request #" << i + 1
                          << ": " << e.what() << "\n";
            }
        }

        auto client_end = std::chrono::steady_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            client_end - client_start);

        double avg_throughput = (total_stats.bytes_transferred / (1024.0 * 1024.0)) /
                               (total_time.count() / 1000.0);

        std::cout << "\n[Client] ===== FINAL STATISTICS =====\n"
                  << "  Total requests sent: " << total_stats.requests_sent << "\n"
                  << "  Total bytes transferred: " << total_stats.bytes_transferred
                  << " (" << (total_stats.bytes_transferred / (1024.0 * 1024.0)) << " MB)\n"
                  << "  Total benchmark time: " << total_time.count() << "ms\n"
                  << "  Average throughput: " << std::fixed << std::setprecision(2)
                  << avg_throughput << " MB/s\n"
                  << "  Average request time: "
                  << (total_time.count() / static_cast<double>(total_stats.requests_sent))
                  << "ms\n"
                  << "  Network efficiency: "
                  << std::fixed << std::setprecision(1)
                  << (100.0 * total_stats.duration.count() / total_time.count())
                  << "%\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "[Client] Fatal error: " << e.what() << std::endl;
        return 1;
    }
}