#include "athena/providers/DevelopmentProvider.h"

namespace athena::providers
{

athena::core::ChatResult DevelopmentProvider::chat(const athena::core::ChatRequest& request)
{
    return athena::core::ChatResult{.answer = "Athena received: " + request.prompt,
                                    .model = "development-provider",
                                    .latencyMs = 0};
}

bool DevelopmentProvider::isReady() const
{
    return true;
}

}  // namespace athena::providers