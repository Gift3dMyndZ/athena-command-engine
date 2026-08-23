#pragma once

#include <chrono>
#include <string>

#include "athena/providers/ILLMProvider.h"

namespace athena::providers
{

class LlamaCppProvider final : public ILLMProvider
{
   public:
    explicit LlamaCppProvider(std::string baseUrl,
                              std::chrono::milliseconds timeout = std::chrono::seconds{30});

    athena::core::ChatResult chat(const athena::core::ChatRequest& request) override;

    [[nodiscard]] bool isReady() const override;

   private:
    std::string baseUrl_;
    std::chrono::milliseconds timeout_;
};

}  // namespace athena::providers