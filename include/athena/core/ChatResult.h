#pragma once

#include <cstdint>
#include <string>

namespace athena::core
{

struct ChatResult
{
    std::string answer;
    std::string model;
    std::uint64_t latencyMs{0};
};

}  // namespace athena::core