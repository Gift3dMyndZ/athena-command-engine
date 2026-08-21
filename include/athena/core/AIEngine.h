#pragma once

#include <memory>

#include "athena/core/ChatRequest.h"
#include "athena/core/ChatResult.h"
#include "athena/providers/ILLMProvider.h"

namespace athena::core
{

class AIEngine
{
   public:
    explicit AIEngine(std::shared_ptr<athena::providers::ILLMProvider> provider);

    [[nodiscard]] ChatResult process(const ChatRequest& request) const;

    [[nodiscard]] bool isReady() const;

   private:
    static void validate(const ChatRequest& request);

    std::shared_ptr<athena::providers::ILLMProvider> provider_;
};

}  // namespace athena::core