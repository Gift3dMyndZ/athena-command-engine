#include "athena/api/ErrorResponse.h"

namespace athena::api
{

std::string toString(ErrorCode code)
{
    switch (code)
    {
        case ErrorCode::InvalidJson:
            return "INVALID_JSON";

        case ErrorCode::ValidationFailed:
            return "VALIDATION_FAILED";

        case ErrorCode::ProviderUnavailable:
            return "PROVIDER_UNAVAILABLE";

        case ErrorCode::ProviderTimeout:
            return "PROVIDER_TIMEOUT";

        case ErrorCode::InternalError:
            return "INTERNAL_ERROR";
    }

    return "INTERNAL_ERROR";
}

drogon::HttpResponsePtr makeErrorResponse(const ApiError& error, const std::string& requestId)
{
    Json::Value body;
    body["error"]["code"] = toString(error.code());
    body["error"]["message"] = error.what();
    body["error"]["requestId"] = requestId;

    auto response = drogon::HttpResponse::newHttpJsonResponse(body);

    response->setStatusCode(static_cast<drogon::HttpStatusCode>(error.httpStatus()));

    response->addHeader("X-Request-ID", requestId);

    return response;
}

drogon::HttpResponsePtr makeUnexpectedErrorResponse(const std::string& requestId)
{
    return makeErrorResponse(
        ApiError{ErrorCode::InternalError, 500, "An unexpected server error occurred."}, requestId);
}

}  // namespace athena::api