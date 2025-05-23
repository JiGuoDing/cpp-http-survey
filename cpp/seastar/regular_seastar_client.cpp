#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/net/api.hh>
#include <seastar/util/log.hh>

static seastar::logger client_log("client");

constexpr size_t DATA_SIZE = 30 * 1024 * 1024;
constexpr size_t LOG_INTERVAL = 1024 * 1024;

seastar::future<> client_connect() {
    seastar::socket_address addr = seastar::make_ipv4_address({"127.0.0.1", 1234});
    client_log.info("Attempting to connect to server at {}", addr);

    return seastar::engine().net().connect(addr).then([] (seastar::connected_socket socket) {
        client_log.info("Successfully connected to server");

        auto out = socket.output();
        auto in = socket.input();

        return seastar::do_with(std::move(out), std::move(in), size_t(0), size_t(0),
            [] (auto& out, auto& in, auto& total_received, auto& last_logged) -> seastar::future<> {
                client_log.info("Starting to receive data...");

                return seastar::do_until(
                    [&total_received] { return total_received >= DATA_SIZE; },
                    [&in, &total_received, &last_logged] () -> seastar::future<> {
                        return in.read().then([&total_received, &last_logged] (auto data) {
                            if (data.empty()) {
                                client_log.warn("Connection closed by server");
                                return seastar::make_ready_future<>();
                            }

                            total_received += data.size();

                            if (total_received - last_logged >= LOG_INTERVAL) {
                                client_log.info("Received {}/{} bytes ({}%)",
                                    total_received, DATA_SIZE,
                                    total_received * 100 / DATA_SIZE);
                                last_logged = total_received;
                            }

                            return seastar::make_ready_future<>();
                        });
                    }
                ).then([&total_received] {
                    client_log.info("Transfer complete. Total received: {} bytes", total_received);
                    return seastar::make_ready_future<>();
                }).then([&out] {
                    client_log.info("Closing output...");
                    return out.close();
                }).then([&in] {
                    client_log.info("Closing input...");
                    return in.close();
                }).handle_exception([] (std::exception_ptr ep) {
                    try {
                        std::rethrow_exception(ep);
                    } catch (const std::exception& ex) {
                        client_log.error("Error during transfer: {}", ex.what());
                    }
                    return seastar::make_ready_future<>();
                });
            });
    }).handle_exception([] (std::exception_ptr ep) {
        try {
            std::rethrow_exception(ep);
        } catch (const std::exception& ex) {
            client_log.error("Connection error: {}", ex.what());
        }
        return seastar::make_ready_future<>();
    });
}

int main(int argc, char** argv) {
    seastar::app_template app;
    seastar::global_logger_registry().set_all_loggers_level(seastar::log_level::debug);

    try {
        app.run(argc, argv, [] {
            return client_connect();
        });
    } catch (const std::exception& ex) {
        client_log.error("Client fatal error: {}", ex.what());
        return 1;
    }
    return 0;
}