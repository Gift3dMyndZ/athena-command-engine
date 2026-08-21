#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace athena::api
{

enum class ErrorCode
{
    InvalidJson,
    ValidationFailed,
    ProviderUnavailable,
    ProviderTimeout,
    InternalError
};

class ApiError final : public std::runtime_error
{
public:
    ApiError(ErrorCode code, int httpStatus, std::string message)
        : std::runtime_error(std::move(message)),
          code_(code),
          httpStatus_(httpStatus)
    {
    }

    [[nodiscard]] ErrorCode code() const noexcept
    {
        return code_;
    }

    [[nodiscard]] int httpStatus() const noexcept
    {
        return httpStatus_;
    }

private:
    ErrorCode code_;
    int httpStatus_;
};

}  // namespace athena::api