#include <drogon/HttpTypes.h>
#include <gtest/gtest.h>

#include <string>

#include "athena/api/ApiError.h"
#include "athena/api/ErrorResponse.h"

namespace
{

using athena::api::ApiError;
using athena::api::ErrorCode;
using athena::api::makeErrorResponse;
using athena::api::makeUnexpectedErrorResponse;
using athena::api::toString;

TEST(ErrorResponseTest, ConvertsInvalidJsonCode)
{
    EXPECT_EQ(toString(ErrorCode::InvalidJson), "INVALID_JSON");
}

TEST(ErrorResponseTest, ConvertsValidationFailedCode)
{
    EXPECT_EQ(toString(ErrorCode::ValidationFailed), "VALIDATION_FAILED");
}

TEST(ErrorResponseTest, ConvertsProviderUnavailableCode)
{
    EXPECT_EQ(toString(ErrorCode::ProviderUnavailable), "PROVIDER_UNAVAILABLE");
}

TEST(ErrorResponseTest, ConvertsProviderTimeoutCode)
{
    EXPECT_EQ(toString(ErrorCode::ProviderTimeout), "PROVIDER_TIMEOUT");
}

TEST(ErrorResponseTest, ConvertsInternalErrorCode)
{
    EXPECT_EQ(toString(ErrorCode::InternalError), "INTERNAL_ERROR");
}

TEST(ErrorResponseTest, CreatesStructuredErrorResponse)
{
    const std::string requestId{"test-request-123"};

    const ApiError error{ErrorCode::ValidationFailed, 400, "The prompt field is required."};

    const auto response = makeErrorResponse(error, requestId);

    ASSERT_NE(response, nullptr);

    EXPECT_EQ(response->statusCode(), drogon::k400BadRequest);

    EXPECT_EQ(response->getHeader("X-Request-ID"), requestId);

    const auto& json = response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "VALIDATION_FAILED");

    EXPECT_EQ((*json)["error"]["message"].asString(), "The prompt field is required.");

    EXPECT_EQ((*json)["error"]["requestId"].asString(), requestId);
}

TEST(ErrorResponseTest, CreatesSafeUnexpectedErrorResponse)
{
    const std::string requestId{"unexpected-error-456"};

    const auto response = makeUnexpectedErrorResponse(requestId);

    ASSERT_NE(response, nullptr);

    EXPECT_EQ(response->statusCode(), drogon::k500InternalServerError);

    EXPECT_EQ(response->getHeader("X-Request-ID"), requestId);

    const auto& json = response->getJsonObject();

    ASSERT_NE(json, nullptr);
    ASSERT_TRUE(json->isMember("error"));

    EXPECT_EQ((*json)["error"]["code"].asString(), "INTERNAL_ERROR");

    EXPECT_EQ((*json)["error"]["message"].asString(), "An unexpected server error occurred.");

    EXPECT_EQ((*json)["error"]["requestId"].asString(), requestId);
}

}  // namespace