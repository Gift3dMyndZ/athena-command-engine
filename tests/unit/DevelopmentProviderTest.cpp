#include <gtest/gtest.h>

#include <string>

#include "athena/core/ChatRequest.h"
#include "athena/providers/DevelopmentProvider.h"

namespace
{

TEST(DevelopmentProviderTest, ReportsReady)
{
    athena::providers::DevelopmentProvider provider;

    EXPECT_TRUE(provider.isReady());
}

TEST(DevelopmentProviderTest, ReturnsDeterministicResult)
{
    athena::providers::DevelopmentProvider provider;

    const athena::core::ChatRequest request{.prompt = "Explain RAII in C++.",
                                            .model = "development-provider",
                                            .temperature = 0.7,
                                            .maxTokens = 512};

    const auto result = provider.chat(request);

    EXPECT_EQ(result.answer, "Athena received: Explain RAII in C++.");

    EXPECT_EQ(result.model, "development-provider");
    EXPECT_EQ(result.latencyMs, 0);
}

TEST(DevelopmentProviderTest, PreservesSubmittedPrompt)
{
    athena::providers::DevelopmentProvider provider;

    const athena::core::ChatRequest request{.prompt = "Describe a Kubernetes readiness probe.",
                                            .model = "development-provider",
                                            .temperature = 0.7,
                                            .maxTokens = 512};

    const auto result = provider.chat(request);

    EXPECT_NE(result.answer.find(request.prompt), std::string::npos);
}

TEST(DevelopmentProviderTest, ProducesRepeatableOutput)
{
    athena::providers::DevelopmentProvider provider;

    const athena::core::ChatRequest request{.prompt = "What is dependency injection?",
                                            .model = "development-provider",
                                            .temperature = 0.7,
                                            .maxTokens = 512};

    const auto firstResult = provider.chat(request);
    const auto secondResult = provider.chat(request);

    EXPECT_EQ(firstResult.answer, secondResult.answer);
    EXPECT_EQ(firstResult.model, secondResult.model);
    EXPECT_EQ(firstResult.latencyMs, secondResult.latencyMs);
}

}  // namespace