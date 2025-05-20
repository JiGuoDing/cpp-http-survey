// regular_transfer.cc
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/distributed.hh>
#include <seastar/net/api.hh>

using namespace seastar;
using namespace net;

constexpr size_t data_size = 30 * 1024 * 1024; // 30MB

future<> handle_connection(lw_shared_ptr<connected_socket> socket) {
    static const char* response = "OK";
    auto out = socket->output();
    auto in = socket->input();
    
    // 接收数据
    return in.read_exactly(data_size).then([out = std::move(out)] (temporary_buffer<char> buf) {
        // 发送响应
        return out.write(response, strlen(response)).then([out] {
            return out.close();
        });
    });
}

future<> service_loop() {
    listen_options lo;
    lo.reuse_address = true;
    return do_with(listen(make_ipv4_address({1234}), lo), [] (auto& listener) {
        return keep_doing([&listener] () {
            return listener.accept().then([] (connected_socket socket, socket_address addr) {
                auto conn = make_lw_shared(std::move(socket));
                handle_connection(conn).finally([conn] {});
            });
        });
    });
}

int main(int argc, char** argv) {
    app_template app;
    app.run(argc, argv, [] {
        return parallel_for_each(engine().cpu_ids(), [] (unsigned id) {
            return smp::submit_to(id, [] {
                return service_loop();
            });
        });
    });
}