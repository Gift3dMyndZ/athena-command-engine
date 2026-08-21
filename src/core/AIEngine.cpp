#include "athena/core/AIEngine.h"

#include <stdexcept>
#include <utility>

#include "athena/api/ApiError.h"

namespace athena::core
{

AIEngine::AIEngine(std::shared_ptr<athena::providers::ILLMProvider> provider)
    : provider_(std::move(provider))
{
    if (!provider_)
    {
        throw std::invalid_argument("AI provider cannot be null");
    }
}

void AIEngine::validate(const ChatRequest& request)
{
    if (request.prompt.empty())
    {
        throw athena::api::ApiError{athena::api::ErrorCode::ValidationFailed, 400,
                                    "The prompt field is required."};
    }

    if (request.prompt.size() > 16000)
    {
        throw athena::api::ApiError{athena::api::ErrorCode::ValidationFailed, 400,
                                    "The prompt exceeds the 16000 character limit."};
    }

    if (request.temperature < 0.0 || request.temperature > 2.0)
    {
        throw athena::api::ApiError{athena::api::ErrorCode::ValidationFailed, 400,
                                    "Temperature must be between 0.0 and 2.0."};
    }

    if (request.maxTokens == 0 || request.maxTokens > 4096)
    {
        throw athena::api::ApiError{athena::api::ErrorCode::ValidationFailed, 400,
                                    "maxTokens must be between 1 and 4096."};
    }
}

ChatResult AIEngine::process(const ChatRequest& request) const
{
    validate(request);

    if (!provider_->isReady())
    {
        throw athena::api::ApiError{athena::api::ErrorCode::ProviderUnavailable, 503,
                                    "The AI provider is not ready."};
    }

    return provider_->chat(request);
}

bool AIEngine::isReady() const
{
    return provider_->isReady();
}

}  // namespace athena::core