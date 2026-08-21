#pragma once

#include <cstddef>
#include <string>

namespace athena::core
{

struct ChatRequest
{
    std::string prompt;
    std::string model{"local-model"};
    double temperature{0.7};
    std::size_t maxTokens{512};
};

}  // namespace athena::core