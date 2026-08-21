#include "athena/api/ApiError.h"
#include "athena/core/AIEngine.h"
#include "athena/providers/ILLMProvider.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

namespace
{

class FakeProvider final : public athena::providers::ILLMProvider
{
public:
    bool ready{true};

    athena::core::ChatResult result{
        .answer = "Test answer from Athena",
        .model = "fake-model",
        .latencyMs = 5};

    athena::core::ChatResult chat(
        const athena::core::ChatRequest&) override
    {
        return result;
    }

    [[nodiscard]] bool isReady() const override
    {
        return ready;
    }
};

}  // namespace

TEST(AIEngineTest, ReturnsProviderResultForValidPrompt)
{
    auto provider = std::make_shared<FakeProvider>();
    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = "Hello, Athena";

    const auto result = engine.process(request);

    EXPECT_EQ(result.answer, "Test answer from Athena");
    EXPECT_EQ(result.model, "fake-model");
    EXPECT_EQ(result.latencyMs, 5);
}

TEST(AIEngineTest, RejectsEmptyPrompt)
{
    auto provider = std::make_shared<FakeProvider>();
    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = "";

    EXPECT_THROW(
        static_cast<void>(engine.process(request)),
        athena::api::ApiError);
}

TEST(AIEngineTest, RejectsOversizedPrompt)
{
    auto provider = std::make_shared<FakeProvider>();
    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = std::string(16001, 'A');

    EXPECT_THROW(
        static_cast<void>(engine.process(request)),
        athena::api::ApiError);
}

TEST(AIEngineTest, RejectsInvalidTemperature)
{
    auto provider = std::make_shared<FakeProvider>();
    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = "Hello";
    request.temperature = 3.0;

    EXPECT_THROW(
        static_cast<void>(engine.process(request)),
        athena::api::ApiError);
}

TEST(AIEngineTest, RejectsInvalidMaximumTokenCount)
{
    auto provider = std::make_shared<FakeProvider>();
    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = "Hello";
    request.maxTokens = 0;

    EXPECT_THROW(
        static_cast<void>(engine.process(request)),
        athena::api::ApiError);
}

TEST(AIEngineTest, ReportsUnavailableProvider)
{
    auto provider = std::make_shared<FakeProvider>();
    provider->ready = false;

    athena::core::AIEngine engine{provider};

    athena::core::ChatRequest request;
    request.prompt = "Hello";

    try
    {
        static_cast<void>(engine.process(request));
        FAIL() << "Expected ApiError";
    }
    catch (const athena::api::ApiError& error)
    {
        EXPECT_EQ(error.httpStatus(), 503);
        EXPECT_EQ(
            error.code(),
            athena::api::ErrorCode::ProviderUnavailable);
    }
}

TEST(AIEngineTest, RejectsNullProvider)
{
    EXPECT_THROW(
        athena::core::AIEngine{nullptr},
        std::invalid_argument);
}