#pragma once

#include "athena/core/ChatRequest.h"
#include "athena/core/ChatResult.h"

namespace athena::providers
{

class ILLMProvider
{
public:
    virtual ~ILLMProvider() = default;

    virtual athena::core::ChatResult chat(
        const athena::core::ChatRequest& request) = 0;

    [[nodiscard]] virtual bool isReady() const = 0;
};

}  // namespace athena::providers