//
// Created by jgd on 25-5-19.
//

#include <drogon/drogon.h>

int main()
{
	// 创建异步 HTTP 客户端
	auto client = drogon::HttpClient::newHttpClient("http://20.2.90.172:9873");

	// 创建请求
	auto req = drogon::HttpRequest::newHttpRequest();
	req->setMethod(drogon::Get);
	req->setPath("/hello");

	// 发送 GET 请求到 /hello
	client->sendRequest(
		req,
		[](drogon::ReqResult result, const drogon::HttpResponsePtr &resp)
		{
			if (result != drogon::ReqResult::Ok)
			{
				std::cerr << "Request failed: " << result << std::endl;
				return;
			}
			std::cout << "Server response: " << resp->getBody() << std::endl;
		});

	drogon::app().run();
	return 0;
}