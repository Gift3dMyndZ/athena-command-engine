#include "athena/api/ChatController.h"

#include <exception>
#include <memory>
#include <random>
#include <sstream>
#include <string>

#include "athena/api/ErrorResponse.h"
#include "athena/providers/DevelopmentProvider.h"

namespace
{

std::string makeRequestId()
{
    static thread_local std::mt19937_64 generator{std::random_device{}()};

    std::ostringstream stream;
    stream << std::hex << generator();

    return stream.str();
}

}  // namespace

namespace athena::api
{

ChatController::ChatController()
    : engine_(std::make_shared<athena::core::AIEngine>(
          std::make_shared<athena::providers::DevelopmentProvider>()))
{
}

void ChatController::chat(const drogon::HttpRequestPtr& request,
                          std::function<void(const drogon::HttpResponsePtr&)>&& callback) const
{
    const std::string requestId = makeRequestId();

    try
    {
        const auto json = request->getJsonObject();

        if (!json)
        {
            throw ApiError{ErrorCode::InvalidJson, 400,
                           "The request body must contain valid JSON."};
        }

        athena::core::ChatRequest chatRequest;
        chatRequest.prompt = json->get("prompt", "").asString();
        chatRequest.model = json->get("model", "development-provider").asString();
        chatRequest.temperature = json->get("temperature", 0.7).asDouble();
        chatRequest.maxTokens = json->get("maxTokens", 512).asUInt64();

        const auto result = engine_->process(chatRequest);

        Json::Value body;
        body["data"]["answer"] = result.answer;
        body["data"]["model"] = result.model;
        body["data"]["latencyMs"] = Json::UInt64(result.latencyMs);
        body["meta"]["requestId"] = requestId;

        auto response = drogon::HttpResponse::newHttpJsonResponse(body);

        response->setStatusCode(drogon::k200OK);
        response->addHeader("X-Request-ID", requestId);

        callback(response);
    }
    catch (const ApiError& error)
    {
        callback(makeErrorResponse(error, requestId));
    }
    catch (const std::exception&)
    {
        callback(makeUnexpectedErrorResponse(requestId));
    }
    catch (...)
    {
        callback(makeUnexpectedErrorResponse(requestId));
    }
}

}  // namespace athena::api