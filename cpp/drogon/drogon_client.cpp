#include <drogon/drogon.h>
#include <chrono>
#include <vector>
#include <numeric>
#include <iostream>
#include <iomanip>

struct TestResult {
    size_t dataSize;
    double duration;
    double throughput;
};

void printResults(const std::vector<TestResult>& results, const std::string& testType) {
    if (results.empty()) return;

    // 计算平均值
    double avgDuration = std::accumulate(results.begin(), results.end(), 0.0,
        [](double sum, const TestResult& r) { return sum + r.duration; }) / results.size();
    
    double avgThroughput = std::accumulate(results.begin(), results.end(), 0.0,
        [](double sum, const TestResult& r) { return sum + r.throughput; }) / results.size();

    // 打印结果
    std::cout << "\n=== " << testType << " Test Results ===\n";
    std::cout << "Test runs: " << results.size() << "\n";
    std::cout << "Data size: " << results[0].dataSize / (1024 * 1024) << " MB\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average duration: " << avgDuration << " seconds\n";
    std::cout << "Average throughput: " << avgThroughput << " MB/s\n\n";

    // 打印每次测试的详细结果
    std::cout << "Detailed results:\n";
    std::cout << "Run\tDuration(s)\tThroughput(MB/s)\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i+1 << "\t" << results[i].duration << "\t\t" 
                  << results[i].throughput << "\n";
    }
}

void testRegularData(const drogon::HttpClientPtr& client, int testCount) {
    std::vector<TestResult> results;
    
    for (int i = 0; i < testCount; ++i) {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setMethod(drogon::Get);
        req->setPath("/data");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        TestResult result;
        
        client->sendRequest(
            req,
            [&result, &start](drogon::ReqResult res, const drogon::HttpResponsePtr& resp) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                
                if (res != drogon::ReqResult::Ok || !resp) {
                    std::cerr << "Request failed: " << res << std::endl;
                    return;
                }
                
                result.dataSize = resp->getBody().size();
                result.duration = duration.count();
                result.throughput = (result.dataSize / (1024.0 * 1024.0)) / result.duration;
            });
        
        // 等待请求完成
        while (result.dataSize == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        results.push_back(result);
        std::cout << "Regular data test " << i+1 << " completed: " 
                  << result.duration << " seconds\n";
    }
    
    printResults(results, "Regular Data");
}

// void testStreamData(const drogon::HttpClientPtr& client, int testCount) {
//     std::vector<TestResult> results;
//
//     for (int i = 0; i < testCount; ++i) {
//         auto req = drogon::HttpRequest::newHttpRequest();
//         req->setMethod(drogon::Get);
//         req->setPath("/stream");
//
//         auto start = std::chrono::high_resolution_clock::now();
//         size_t totalReceived = 0;
//         bool completed = false;
//
//         // 创建流响应处理器
//         auto resp = drogon::HttpResponse::newHttpResponse();
//         resp->setOnDataReceivedCallback([&totalReceived](const char* data, size_t len) {
//             totalReceived += len;
//             return true;
//         });
//
//         client->sendRequest(
//             req,
//             [&totalReceived, &start, &results, i, &completed, resp](drogon::ReqResult res, const drogon::HttpResponsePtr& response) {
//                 auto end = std::chrono::high_resolution_clock::now();
//                 std::chrono::duration<double> duration = end - start;
//
//                 if (res != drogon::ReqResult::Ok || !response) {
//                     std::cerr << "Request failed: " << res << std::endl;
//                     completed = true;
//                     return;
//                 }
//
//                 TestResult result;
//                 result.dataSize = totalReceived;
//                 result.duration = duration.count();
//                 result.throughput = (result.dataSize / (1024.0 * 1024.0)) / result.duration;
//
//                 results.push_back(result);
//                 std::cout << "Stream data test " << i+1 << " completed: "
//                           << result.duration << " seconds\n";
//                 completed = true;
//             });
//
//         // 等待请求完成
//         while (!completed) {
//             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         }
//     }
//
//     printResults(results, "Stream Data");
// }

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <test_count>\n";
        return 1;
    }

    std::string serverAddress = argv[1];
    int testCount = std::stoi(argv[2]);

    // 创建HTTP客户端
    auto client = drogon::HttpClient::newHttpClient(serverAddress, 9873);

    // 测试普通数据传输
    testRegularData(client, testCount);

    // 测试流数据传输
    // testStreamData(client, testCount);

    return 0;
}