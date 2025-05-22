#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

struct Stats {
    size_t bytes_transferred = 0;
    std::chrono::milliseconds duration = std::chrono::milliseconds(0);
    size_t requests_processed = 0;
};

void log_request(const http::request<http::string_body>& req) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << "\n[Server] " << std::put_time(std::localtime(&now_c), "%F %T")
              << " | Received request:\n"
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
    std::cout << "[Server] Sending response:\n"
              << "  Status: " << res.result_int() << " " << res.reason() << "\n"
              << "  Version: " << res.version() / 10 << "." << res.version() % 10 << "\n"
              << "  Headers:\n";

    for (const auto& field : res) {
        std::cout << "    " << field.name_string() << ": " << field.value() << "\n";
    }
    std::cout << "  Body size: " << res.body().size() << " bytes\n";
}

void handle_request(http::request<http::string_body>& req, Stats& stats) {
    auto start = std::chrono::steady_clock::now();
    stats.bytes_transferred += req.body().size();
    auto end = std::chrono::steady_clock::now();
    stats.duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    stats.requests_processed++;
}

void run_server(net::io_context& ioc, unsigned short port) {
    tcp::acceptor acceptor(ioc, {tcp::v4(), port});
    Stats total_stats;
    auto server_start = std::chrono::steady_clock::now();

    std::cout << "[Server] Starting at 0.0.0.0:" << port
              << " | Max body size: 100MB\n";

    for (int i = 0; i < 5; ++i) {
        try {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request_parser<http::string_body> parser;
            parser.body_limit(100 * 1024 * 1024);

            auto req_start = std::chrono::steady_clock::now();
            http::read(socket, buffer, parser);
            auto req = parser.release();
            auto req_end = std::chrono::steady_clock::now();

            log_request(req);

            std::cout << "[Server] Request parsing time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(req_end - req_start).count()
                      << "ms\n";

            Stats stats;
            handle_request(req, stats);
            total_stats.bytes_transferred += stats.bytes_transferred;
            total_stats.duration += stats.duration;

            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, "Beast Server");
            res.set(http::field::content_type, "text/plain");
            res.body() = "Data received (" + std::to_string(stats.bytes_transferred) + " bytes)";
            res.prepare_payload();

            log_response(res);

            http::write(socket, res);

            double throughput = (stats.bytes_transferred / (1024.0 * 1024.0)) /
                               (stats.duration.count() / 1000.0);

            std::cout << "[Server] Request #" << i + 1 << " stats:\n"
                      << "  Bytes: " << stats.bytes_transferred << "\n"
                      << "  Time: " << stats.duration.count() << "ms\n"
                      << "  Throughput: " << std::fixed << std::setprecision(2)
                      << throughput << " MB/s\n";
        } catch (const std::exception& e) {
            std::cerr << "[Server] Error processing request #" << i + 1
                      << ": " << e.what() << "\n";
        }
    }

    auto server_end = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        server_end - server_start);

    double avg_throughput = (total_stats.bytes_transferred / (1024.0 * 1024.0)) /
                           (total_time.count() / 1000.0);

    std::cout << "\n[Server] ===== FINAL STATISTICS =====\n"
              << "  Total requests processed: " << total_stats.requests_processed << "\n"
              << "  Total bytes received: " << total_stats.bytes_transferred
              << " (" << (total_stats.bytes_transferred / (1024.0 * 1024.0)) << " MB)\n"
              << "  Total processing time: " << total_time.count() << "ms\n"
              << "  Average throughput: " << std::fixed << std::setprecision(2)
              << avg_throughput << " MB/s\n"
              << "  Average request time: "
              << (total_time.count() / static_cast<double>(total_stats.requests_processed))
              << "ms\n";
}

int main() {
    try {
        net::io_context ioc;
        run_server(ioc, 8080);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "[Server] Fatal error: " << e.what() << std::endl;
        return 1;
    }
}