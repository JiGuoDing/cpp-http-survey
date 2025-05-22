#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/net/api.hh>
#include <seastar/core/temporary_buffer.hh>
#include <seastar/util/log.hh>

// 定义日志器
static seastar::logger server_log("server");

constexpr size_t DATA_SIZE = 30 * 1024 * 1024; // 30MB

seastar::future<> handle_connection(seastar::connected_socket socket) {
    auto out = socket.output();
    auto in = socket.input();

    server_log.info("Starting to handle new connection");

    // 创建一个30MB的数据缓冲区
    seastar::temporary_buffer<char> data(DATA_SIZE);
    std::fill(data.get_write(), data.get_write() + DATA_SIZE, 'A'); // 填充数据

    server_log.info("Created 30MB data buffer, starting to send data...");

    // 发送数据
    try {
        co_await out.write(data.get(), data.size());
        server_log.info("Data sent, flushing...");
        co_await out.flush();
        server_log.info("Data flushed successfully");
    } catch (const std::exception& ex) {
        server_log.error("Error sending data: {}", ex.what());
        throw;
    }

    // 关闭连接
    try {
        server_log.info("Closing connection...");
        co_await out.close();
        co_await in.close();
        server_log.info("Connection closed gracefully");
    } catch (const std::exception& ex) {
        server_log.error("Error closing connection: {}", ex.what());
        throw;
    }
}

seastar::future<> service_loop() {
    seastar::listen_options lo;
    lo.reuse_address = true;

    server_log.info("Starting server on port 1234");

    return seastar::do_with(seastar::listen(seastar::make_ipv4_address({"0.0.0.0", 1234}), lo),
        [] (auto& listener) {
            return seastar::keep_doing([&listener] () {
                return listener.accept().then(
                    [] (seastar::accept_result result) {
                        auto [socket, addr] = std::move(result);
                        server_log.info("Accepted connection from {}", addr);
                        return handle_connection(std::move(socket)).handle_exception(
                            [] (std::exception_ptr ep) {
                                try {
                                    std::rethrow_exception(ep);
                                    return seastar::make_ready_future<>();
                                } catch (const std::exception& ex) {
                                    server_log.error("Connection error: {}", ex.what());
                                }
                            });
                    }).handle_exception([] (std::exception_ptr ep) {
                        try {
                            std::rethrow_exception(ep);
                        } catch (const std::exception& ex) {
                            server_log.error("Accept error: {}", ex.what());
                        }
                    });
            });
        });
}

int main(int argc, char** argv) {
    seastar::app_template app;

    // 配置日志级别
    seastar::global_logger_registry().set_all_loggers_level(seastar::log_level::debug);

    try {
        app.run(argc, argv, [] {
            return service_loop();
        });
    } catch (const std::exception& ex) {
        server_log.error("Server fatal error: {}", ex.what());
        return 1;
    }
    return 0;
}