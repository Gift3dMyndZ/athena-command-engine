#pragma once

#include <drogon/HttpResponse.h>

#include <string>

#include "athena/api/ApiError.h"

namespace athena::api
{

[[nodiscard]] std::string toString(ErrorCode code);

[[nodiscard]] drogon::HttpResponsePtr makeErrorResponse(const ApiError& error,
                                                        const std::string& requestId);

[[nodiscard]] drogon::HttpResponsePtr makeUnexpectedErrorResponse(const std::string& requestId);

}  // namespace athena::api