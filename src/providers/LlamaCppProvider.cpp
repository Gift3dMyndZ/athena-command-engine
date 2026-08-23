#include "athena/providers/LlamaCppProvider.h"

#include <stdexcept>
#include <utility>

namespace athena::providers
{

LlamaCppProvider::LlamaCppProvider(std::string baseUrl, const std::chrono::milliseconds timeout)
    : baseUrl_(std::move(baseUrl)), timeout_(timeout)
{
    if (baseUrl_.empty())
    {
        throw std::invalid_argument{"The llama.cpp base URL must not be empty."};
    }

    if (timeout_ <= std::chrono::milliseconds::zero())
    {
        throw std::invalid_argument{"The llama.cpp timeout must be greater than zero."};
    }

    while (baseUrl_.size() > 1 && baseUrl_.back() == '/')
    {
        baseUrl_.pop_back();
    }
}

athena::core::ChatResult LlamaCppProvider::chat(const athena::core::ChatRequest&)
{
    throw std::logic_error{"LlamaCppProvider chat is not implemented yet."};
}

bool LlamaCppProvider::isReady() const
{
    return false;
}

}  // namespace athena::providers