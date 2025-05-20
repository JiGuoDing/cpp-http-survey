#include <iostream>
#include "../../headers/httplib.h"

constexpr size_t data_size = 30 * 1024 * 1024;

void handle_post(const httplib::Request& req, httplib::Response& res) {
    // 获取请求体数据
    std::string body = req.body;

    // 再服务器端打印请求体数据
    std::cout << "Received POST request with body: " << body << std::endl;

    // 设置响应内容
    res.set_content("POST request received", "application/json");
}

int main()
{
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.Get("/about", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("About page", "text/plain");
    });

    svr.Post("/api/users", handle_post);

    std::cout << "Server started on port 9873" << std::endl;

    svr.listen("0.0.0.0", 9873);
    return 0;
}
