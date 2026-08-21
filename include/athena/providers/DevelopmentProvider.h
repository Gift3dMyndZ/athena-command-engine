#pragma once

#include "athena/providers/ILLMProvider.h"

namespace athena::providers
{

class DevelopmentProvider final : public ILLMProvider
{
   public:
    athena::core::ChatResult chat(const athena::core::ChatRequest& request) override;

    [[nodiscard]] bool isReady() const override;
};

}  // namespace athena::providers