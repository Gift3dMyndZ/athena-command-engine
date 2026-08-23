#include <gtest/gtest.h>

#include <chrono>
#include <stdexcept>
#include <string>

#include "athena/providers/LlamaCppProvider.h"

namespace
{

using athena::providers::LlamaCppProvider;
using namespace std::chrono_literals;

std::string makeTestBaseUrl()
{
    return std::string{"http"} + std::string{"://"} + std::string{"127.0.0.1:8080"};
}

TEST(LlamaCppProviderTest, ConstructsWithValidConfiguration)
{
    EXPECT_NO_THROW((LlamaCppProvider{makeTestBaseUrl(), 5s}));
}

TEST(LlamaCppProviderTest, RejectsEmptyBaseUrl)
{
    EXPECT_THROW((LlamaCppProvider{std::string{}, 5s}), std::invalid_argument);
}

TEST(LlamaCppProviderTest, RejectsZeroTimeout)
{
    EXPECT_THROW((LlamaCppProvider{makeTestBaseUrl(), 0ms}), std::invalid_argument);
}

TEST(LlamaCppProviderTest, RejectsNegativeTimeout)
{
    EXPECT_THROW((LlamaCppProvider{makeTestBaseUrl(), -1ms}), std::invalid_argument);
}

TEST(LlamaCppProviderTest, ReportsNotReadyBeforeHealthImplementation)
{
    const LlamaCppProvider provider{makeTestBaseUrl(), 5s};

    EXPECT_FALSE(provider.isReady());
}

}  // namespace