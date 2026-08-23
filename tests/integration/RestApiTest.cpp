#include <drogon/HttpClient.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <gtest/gtest.h>
#include <trantor/net/EventLoop.h>
#include <trantor/net/EventLoopThread.h>

#include <cstdlib>
#include <string>
#include <utility>

namespace
{

constexpr const char* defaultBaseUrl{"http://127.0.0.1:8848"};

std::string getBaseUrl()
{
    const char* configuredUrl = std::getenv("ATHENA_TEST_BASE_URL");

    if (configuredUrl != nullptr && !std::string{configuredUrl}.empty())
    {
        return configuredUrl;
    }

    return defaultBaseUrl;
}

trantor::EventLoop* getHttpClientLoop()
{
    static trantor::EventLoopThread loopThread;

    static const bool started = (loopThread.run(), true);

    static_cast<void>(started);

    return loopThread.getLoop();
}

struct HttpResult
{
    drogon::ReqResult transportResult;
    drogon::HttpResponsePtr response;
};

HttpResult sendRequest(const drogon::HttpRequestPtr& request, const double timeoutSeconds = 5.0)
{
    const auto client = drogon::HttpClient::newHttpClient(getBaseUrl(), getHttpClientLoop());

    auto [transportResult, response] = client->sendRequest(request, timeoutSeconds);

    return HttpResult{.transportResult = transportResult, .response = std::move(response)};
}

drogon::HttpRequestPtr makeGetRequest(const std::string& path)
{
    const auto request = drogon::HttpRequest::newHttpRequest();

    request->setMethod(drogon::Get);
    request->setPath(path);

    return request;
}

drogon::HttpRequestPtr makeJsonPostRequest(const std::string& path, const std::string& body)
{
    const auto request = drogon::HttpRequest::newHttpRequest();

    request->setMethod(drogon::Post);
    request->setPath(path);
    request->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    request->setBody(body);

    return request;
}

void expectSuccessfulTransport(const HttpResult& result)
{
    ASSERT_EQ(result.transportResult, drogon::ReqResult::Ok);

    ASSERT_NE(result.response, nullptr);
}

TEST(RestApiTest, HealthEndpointReturnsHealthyStatus)
{
    const auto result = sendRequest(makeGetRequest("/api/v1/health"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k200OK);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);

    EXPECT_EQ((*json)["status"].asString(), "healthy");

    EXPECT_EQ((*json)["service"].asString(), "athena-command-engine");

    EXPECT_EQ((*json)["version"].asString(), "0.1.0");
}

TEST(RestApiTest, ValidChatRequestReturnsAnswer)
{
    const auto result =
        sendRequest(makeJsonPostRequest("/api/v1/chat", R"({"prompt":"Explain RAII in C++."})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k200OK);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("data"));
    ASSERT_TRUE(json->isMember("meta"));

    EXPECT_EQ((*json)["data"]["answer"].asString(), "Athena received: Explain RAII in C++.");

    EXPECT_EQ((*json)["data"]["model"].asString(), "development-provider");

    EXPECT_TRUE((*json)["data"].isMember("latencyMs"));

    EXPECT_FALSE((*json)["meta"]["requestId"].asString().empty());
}

TEST(RestApiTest, RequestIdMatchesHeaderAndBody)
{
    const auto result =
        sendRequest(makeJsonPostRequest("/api/v1/chat", R"({"prompt":"Check request ID."})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k200OK);

    const std::string headerRequestId = result.response->getHeader("X-Request-ID");

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);

    const std::string bodyRequestId = (*json)["meta"]["requestId"].asString();

    EXPECT_FALSE(headerRequestId.empty());
    EXPECT_FALSE(bodyRequestId.empty());

    EXPECT_EQ(headerRequestId, bodyRequestId);
}

TEST(RestApiTest, MissingPromptReturnsValidationError)
{
    const auto result = sendRequest(makeJsonPostRequest("/api/v1/chat", R"({})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k400BadRequest);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "VALIDATION_FAILED");

    EXPECT_EQ((*json)["error"]["message"].asString(), "The prompt field is required.");

    EXPECT_FALSE((*json)["error"]["requestId"].asString().empty());
}

TEST(RestApiTest, MalformedJsonReturnsInvalidJsonError)
{
    const auto result = sendRequest(makeJsonPostRequest("/api/v1/chat", "not-json"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k400BadRequest);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "INVALID_JSON");

    EXPECT_EQ((*json)["error"]["message"].asString(), "The request body must contain valid JSON.");
}

TEST(RestApiTest, InvalidTemperatureReturnsValidationError)
{
    const auto result =
        sendRequest(makeJsonPostRequest("/api/v1/chat", R"({"prompt":"Hello","temperature":3.0})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k400BadRequest);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "VALIDATION_FAILED");

    EXPECT_EQ((*json)["error"]["message"].asString(), "Temperature must be between 0.0 and 2.0.");
}

TEST(RestApiTest, InvalidMaximumTokensReturnsValidationError)
{
    const auto result =
        sendRequest(makeJsonPostRequest("/api/v1/chat", R"({"prompt":"Hello","maxTokens":0})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k400BadRequest);

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "VALIDATION_FAILED");

    EXPECT_EQ((*json)["error"]["message"].asString(), "maxTokens must be between 1 and 4096.");
}

TEST(RestApiTest, ErrorRequestIdMatchesHeaderAndBody)
{
    const auto result = sendRequest(makeJsonPostRequest("/api/v1/chat", R"({})"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k400BadRequest);

    const std::string headerRequestId = result.response->getHeader("X-Request-ID");

    const auto& json = result.response->getJsonObject();

    ASSERT_NE(json, nullptr);

    const std::string bodyRequestId = (*json)["error"]["requestId"].asString();

    EXPECT_FALSE(headerRequestId.empty());
    EXPECT_FALSE(bodyRequestId.empty());

    EXPECT_EQ(headerRequestId, bodyRequestId);
}

TEST(RestApiTest, UnknownRouteReturnsNotFound)
{
    const auto result = sendRequest(makeGetRequest("/api/v1/not-found"));

    expectSuccessfulTransport(result);

    EXPECT_EQ(result.response->statusCode(), drogon::k404NotFound);
}

}  // namespace