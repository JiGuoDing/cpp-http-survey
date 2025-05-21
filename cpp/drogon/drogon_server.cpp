#include <drogon/drogon.h>
#include <chrono>
#include <vector>
#include <numeric>

// 生成30MB的测试数据
std::string generateTestData(size_t size = 30 * 1024 * 1024) {
    std::string data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        data.push_back(static_cast<char>(i % 256));
    }
    return data;
}

// 全局测试数据
const std::string testData = generateTestData();

int main() {
    // 设置日志级别
    drogon::app().setLogLevel(trantor::Logger::kWarn);

    // 普通数据传输路由
    drogon::app().registerHandler(
        "/data",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(testData);
            resp->setContentTypeCode(drogon::CT_APPLICATION_OCTET_STREAM);
            callback(resp);
        },
        {drogon::Get});

    // 流数据传输路由
    drogon::app().registerHandler(
        "/stream",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            // 创建流式响应
            auto resp = drogon::HttpResponse::newStreamResponse(
                [](char *buffer, size_t bufferSize) -> size_t {
                    static size_t totalSent = 0;
                    const size_t chunkSize = 1 * 1024 * 1024; // 1MB chunks

                    if (totalSent >= testData.size()) {
                        return 0; // 数据发送完毕
                    }

                    size_t remaining = testData.size() - totalSent;
                    size_t currentChunk = std::min(chunkSize, remaining);

                    // 将数据复制到缓冲区
                    std::copy(testData.begin() + totalSent,
                              testData.begin() + totalSent + currentChunk,
                              buffer);

                    totalSent += currentChunk;
                    return currentChunk;
                });

            resp->setContentTypeCode(drogon::CT_APPLICATION_OCTET_STREAM);
            callback(resp);
        },
        {drogon::Get});

    LOG_INFO << "Server running on 0.0.0.0:9873";
    drogon::app().addListener("0.0.0.0", 9873).setThreadNum(4).run();
    return 0;
}