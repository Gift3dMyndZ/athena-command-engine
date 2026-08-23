cmake_minimum_required(VERSION 3.25)

project(
    athena_command_engine
    VERSION 0.1.0
    DESCRIPTION "C++ AI intelligence service for the Platform Engineering Command Center"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ATHENA_BUILD_TESTS "Build Athena unit tests" ON)

# Locate the Homebrew Drogon installation.
find_package(Drogon CONFIG REQUIRED)

add_library(
    athena_core
    STATIC
        src/core/AIEngine.cpp
        src/providers/DevelopmentProvider.cpp
)

target_include_directories(
    athena_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(
    athena_core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(
    athena
    src/main.cpp
    src/api/ChatController.cpp
    src/api/ErrorResponse.cpp
    src/api/HealthController.cpp
)

target_link_libraries(
    athena
    PRIVATE
        athena_core
        Drogon::Drogon
)

target_compile_options(
    athena
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

include(CTest)

if(BUILD_TESTING AND ATHENA_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)

    add_executable(
        athena_unit_tests
        tests/unit/AIEngineTest.cpp
        tests/unit/DevelopmentProviderTest.cpp
        tests/unit/ErrorResponseTest.cpp
        src/api/ErrorResponse.cpp
    )

    target_link_libraries(
        athena_unit_tests
        PRIVATE
            athena_core
            Drogon::Drogon
            GTest::gtest
            GTest::gtest_main
    )

    target_compile_options(
        athena_unit_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

    include(GoogleTest)

    gtest_discover_tests(
        athena_unit_tests
    )

    add_executable(
        athena_rest_api_tests
        tests/integration/RestApiTest.cpp
    )

    target_link_libraries(
        athena_rest_api_tests
        PRIVATE
            Drogon::Drogon
            GTest::gtest
            GTest::gtest_main
    )

    target_compile_options(
        athena_rest_api_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

    gtest_discover_tests(
        athena_rest_api_tests
        TEST_PREFIX "integration."
        DISCOVERY_MODE PRE_TEST
    )
endif()