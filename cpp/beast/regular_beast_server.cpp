#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <chrono>
#include <limits>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::unique_ptr<http::request_parser<http::string_body>> parser_;

public:
    explicit session(tcp::socket&& socket) : stream_(std::move(socket)) {}

    void run() {
        net::dispatch(stream_.get_executor(),
            beast::bind_front_handler(&session::read_request, shared_from_this()));
    }

private:
    void read_request() {
        parser_ = std::make_unique<http::request_parser<http::string_body>>();
        parser_->body_limit(std::numeric_limits<std::uint64_t>::max());

        http::async_read(stream_, buffer_, *parser_,
            beast::bind_front_handler(&session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec) {
            std::cerr << "[Server] Read error: " << ec.message() << "\n";
            return;
        }

        const auto& req = parser_->get();

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << "\n[Server] Received request at " << std::put_time(std::localtime(&now_c), "%F %T")
                  << ", method: " << req.method_string()
                  << ", target: " << req.target()
                  << ", body size: " << req.body().size() << " bytes\n";

        auto res = std::make_shared<http::response<http::string_body>>(
            http::status::ok, req.version());
        res->set(http::field::server, "Beast Server");
        res->set(http::field::content_type, "text/plain");
        res->keep_alive(req.keep_alive());
        res->body() = "Data received. Size: " + std::to_string(req.body().size()) + " bytes.";
        res->prepare_payload();

        http::async_write(stream_, *res,
            beast::bind_front_handler(&session::on_write, shared_from_this(), res, res->need_eof()));
    }

    void on_write(std::shared_ptr<http::response<http::string_body>> /*res*/, bool close, beast::error_code ec, std::size_t) {
        if (ec) {
            std::cerr << "[Server] Write error: " << ec.message() << "\n";
            return;
        }

        if (close) {
            beast::error_code ec_shutdown;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec_shutdown);
            return;
        }

        buffer_.consume(buffer_.size());
        read_request();
    }
};

class listener : public std::enable_shared_from_this<listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    listener(net::io_context& ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(net::make_strand(ioc)) {
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(net::socket_base::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
    }

    void run() { do_accept(); }

private:
    void do_accept() {
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(&listener::on_accept, shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<session>(std::move(socket))->run();
        } else {
            std::cerr << "[Server] Accept error: " << ec.message() << "\n";
        }
        do_accept();
    }
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <address> <port>\n"
                      << "Example: server 0.0.0.0 8080\n";
            return EXIT_FAILURE;
        }

        auto const address = net::ip::make_address(argv[1]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
        const int threads = std::thread::hardware_concurrency();

        net::io_context ioc{threads};

        std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

        std::vector<std::thread> thread_pool;
        for (int i = 0; i < threads; ++i) {
            thread_pool.emplace_back([&ioc] { ioc.run(); });
        }

        std::cout << "[Server] Running on " << address << ":" << port
                  << " with " << threads << " threads\n";

        for (auto& t : thread_pool) t.join();
    } catch (const std::exception& e) {
        std::cerr << "[Server] Fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
