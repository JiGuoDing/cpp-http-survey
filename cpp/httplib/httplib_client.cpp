#include <iostream>
#include "headers/httplib.h"

int main()
{
    httplib::Client cli("localhost", 9873);

    // 发送 GET 请求
    auto res = cli.Get("/");
    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
    }

    res = cli.Get("/about");
    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
    }

    // 发送 POST 请求
    const httplib::Headers headers = {
        // 设置请求头 MIME类型
        {"Content.Type", "application/json"}
    };

    const std::string body = R"({"name": "Guoding Ji", "age": 23})";

    if (auto res1 = cli.Post("/api/users", headers, body, "application/json"); res1 && res1->status == 200) {
        std::cout << "Request successful: " << res1->body << std::endl;
    } else {
        std::cout << "Request failed: " << std::endl;
    }

    return 0;
}