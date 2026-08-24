<div align="center">

<img
  src="https://readme-typing-svg.herokuapp.com?font=Cinzel&size=42&pause=1000&color=00A86B&vCenter=true&center=true&width=950&lines=ATHENA+COMMAND+ENGINE;THE+ENGINE+AWAKENS;THE+ORACLE+CONNECTS;THE+PROVIDER+LEARNS;COMMAND+INTELLIGENCE+ONLINE"
  alt="Athena Command Engine"
/>

# Athena Command Engine

### Native C++23 AI Orchestration for the Platform Engineering Command Center

A resilient AI orchestration service built with **C++23**, **Drogon**, **cpr**, **JsonCpp**, **GoogleTest**, and **llama.cpp**.

<img alt="C++23" src="https://img.shields.io/badge/C%2B%2B-23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" />
<img alt="CMake" src="https://img.shields.io/badge/CMake-3.25%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white" />
<img alt="Drogon" src="https://img.shields.io/badge/REST-Drogon-2C3E50?style=for-the-badge" />
<img alt="llama.cpp" src="https://img.shields.io/badge/Inference-llama.cpp-00A86B?style=for-the-badge" />
<img alt="GoogleTest" src="https://img.shields.io/badge/Tests-GoogleTest-4285F4?style=for-the-badge" />
<img alt="License" src="https://img.shields.io/badge/License-MIT-00A86B?style=for-the-badge" />

</div>

---

# Overview

**Athena Command Engine** is a native C++23 AI orchestration service that provides the intelligence layer for the **Platform Engineering Command Center**.

Athena accepts structured chat requests, validates domain rules, delegates inference through a provider abstraction, and returns predictable JSON responses containing:

- request correlation IDs
- structured error contracts
- latency information
- provider metadata
- operational telemetry

The architecture separates transport, orchestration, and inference concerns to support long-term maintainability, testing, and provider flexibility.

Current releases include a deterministic development provider and the infrastructure necessary for local inference through **llama.cpp**.

---

# Why Athena?

Most AI projects demonstrate prompt engineering.

Athena demonstrates software engineering principles around AI integration:

✅ Dependency Inversion

✅ Provider Abstraction

✅ REST API Architecture

✅ Error Contract Design

✅ Health and Readiness Separation

✅ Request Correlation

✅ Automated Testing

✅ Native C++23 Development

✅ AI Infrastructure Integration

✅ Operational Reliability Patterns

---

# Architecture

```text
Client
   |
   | HTTPS JSON
   v
+----------------------+
|  Drogon Controllers  |
+----------------------+
            |
            v
+----------------------+
|      AIEngine        |
| Validation           |
| Orchestration        |
+----------------------+
            |
            v
+----------------------+
|    ILLMProvider      |
+----------------------+
        |        |
        |        |
        v        v
 Development   LlamaCpp
  Provider      Provider
                    |
                    v
              llama-server
                    |
                    v
              GGUF Model
```

The AI engine depends only on a provider interface.

New inference providers can be introduced without modifying orchestration logic.

---

# Core Components

## REST Controllers

Responsible for the transport layer.

- Parse HTTP requests
- Deserialize JSON
- Attach request IDs
- Translate domain errors into HTTP responses
- Return structured API contracts

Controllers remain intentionally thin.

---

## AIEngine

Responsible for orchestration and domain validation.

Responsibilities include:

- prompt validation
- token limits
- temperature validation
- provider readiness checks
- inference delegation
- typed error generation

The engine has no direct dependency on llama.cpp.

---

## Providers

### DevelopmentProvider

Used for:

- local development
- frontend integration
- demonstrations
- unit testing
- CI pipelines

Provides deterministic responses without requiring an LLM.

### LlamaCppProvider

Responsible for:

- connectivity checks
- readiness detection
- chat completion requests
- timeout handling
- upstream response validation
- error translation

Communicates with a local OpenAI-compatible `llama-server`.

---

# API Endpoints

## Health

```http
GET /api/v1/health
```

Response:

```json
{
  "service": "athena-command-engine",
  "status": "healthy",
  "version": "0.1.0"
}
```

---

## Chat

```http
POST /api/v1/chat
```

Request:

```json
{
  "prompt": "Explain RAII in C++.",
  "model": "local-model",
  "temperature": 0.2,
  "maxTokens": 128
}
```

Current development-provider response:

```json
{
  "data": {
    "answer": "Athena received: Explain RAII in C++.",
    "model": "development-provider",
    "latencyMs": 0
  },
  "meta": {
    "requestId": "b1377bbd79f505ef"
  }
}
```

---

## Readiness (Planned)

```http
GET /api/v1/ready
```

Example:

```json
{
  "status": "ready",
  "provider": "llama.cpp",
  "providerReady": true,
  "model": "local-instruct-model",
  "version": "0.1.0"
}
```

Readiness answers:

> Can Athena currently serve inference?

Health only answers:

> Is Athena running?

---

# Error Contract

Athena exposes stable public error codes.

| Error Code | HTTP Status |
|------------|------------|
| INVALID_JSON | 400 |
| VALIDATION_FAILED | 400 |
| PROVIDER_UNAVAILABLE | 503 |
| PROVIDER_TIMEOUT | 504 |
| PROVIDER_INVALID_RESPONSE | 502 |
| INTERNAL_ERROR | 500 |

Example:

```json
{
  "error": {
    "code": "VALIDATION_FAILED",
    "message": "The prompt field is required.",
    "requestId": "a6ab7354722ae939"
  }
}
```

Internal exceptions are never exposed directly to clients.

---

# Request Correlation

Every request is assigned a correlation ID.

Example:

```http
X-Request-ID: a6ab7354722ae939
```

The same identifier is returned in both headers and JSON responses.

This provides a foundation for:

- operational debugging
- distributed tracing
- troubleshooting
- telemetry correlation

---

# Testing

Athena includes both unit and network-level integration coverage.

```text
AIEngine Tests               7
DevelopmentProvider Tests    4
Error Contract Tests         7
LlamaCppProvider Tests       5
------------------------------
Unit Tests                  23

REST Integration Tests       9
CTest Entries               24
```

Covered behavior includes:

- request validation
- provider availability
- error serialization
- malformed JSON handling
- request correlation
- provider construction
- REST lifecycle automation
- startup readiness polling
- cleanup verification

---

# Quick Start

## Prerequisites

```bash
brew install \
  cmake \
  ninja \
  llvm \
  drogon \
  googletest \
  cpr \
  llama.cpp
```

Verify:

```bash
cmake --version
ninja --version
llama-server --version
```

---

## Clone

```bash
git clone https://github.com/Gift3dMyndZ/athena-command-engine.git
cd athena-command-engine
```

---

## Configure

```bash
cmake --preset macos-debug
```

---

## Build

```bash
cmake --build --preset macos-debug
```

---

## Run

```bash
./build/athena
```

---

## Run Tests

Execute all tests:

```bash
cmake --build --preset macos-debug && \
ctest --preset macos-debug --output-on-failure
```

Run unit tests:

```bash
./build/athena_unit_tests
```

Run REST integration tests:

```bash
./scripts/run_rest_api_tests.sh
```

---

# Configuration

Provider selection is configuration driven.

```bash
ATHENA_PROVIDER=development

ATHENA_PROVIDER=llama.cpp
ATHENA_LLM_URL=http://127.0.0.1:8080
ATHENA_LLM_TIMEOUT_MS=30000
```

Expected behavior:

```text
ATHENA_PROVIDER=development
          |
          v
 DevelopmentProvider

ATHENA_PROVIDER=llama.cpp
          |
          v
  LlamaCppProvider
          |
          v
    llama-server
```

---

# Project Structure

```text
athena-command-engine/
├── CMakeLists.txt
├── CMakePresets.json
├── include/
│   └── athena/
├── src/
├── tests/
│   ├── integration/
│   └── unit/
├── scripts/
└── docs/
```

---

# Engineering Principles

Athena is built around several principles:

1. Separate transport from domain policy.
2. Depend on abstractions instead of runtimes.
3. Treat failures as part of the API contract.
4. Distinguish health from readiness.
5. Preserve request correlation everywhere.
6. Keep testing independent of model files.
7. Design for observability from the beginning.
8. Keep AI infrastructure optional.
9. Publish small, testable increments.
10. Document limitations honestly.

---

# Roadmap

## Athena v0.1.0

### Completed

- C++23 orchestration layer
- Drogon REST API
- Provider abstraction
- DevelopmentProvider
- Structured errors
- Request correlation
- Automated unit testing
- Automated REST testing

### Remaining

- llama.cpp readiness checks
- OpenAI-compatible chat completions
- readiness endpoint
- provider-aware health reporting
- configuration-driven provider selection
- GitHub Actions CI
- real GGUF smoke testing

---

## Athena v0.2.0

- telemetry endpoint
- provider metrics
- request limits
- concurrency controls
- Docker image
- structured provider logging
- hosted deployment configuration

---

# Related Project

Athena is the AI backend powering the Platform Engineering Command Center.

### Platform Engineering Command Center

Repository:

https://github.com/Gift3dMyndZ/Gift3dMyndZ.github.io

Live Site:

https://gift3dmyndz.github.io/

---

# Security

Athena follows several security-first principles:

- never commit credentials
- never commit GGUF model weights
- never commit `.env` files
- validate request limits
- enforce provider timeouts
- restrict CORS origins
- avoid logging full prompts by default
- return safe public error messages

Typical repository exclusions:

```gitignore
/build/
/cmake-build-*/
.env
.env.local
*.gguf
/models/
*.log
*.pid
```

---

# Author

## Joshua Wolfe

GitHub:

https://github.com/Gift3dMyndZ

LinkedIn:

https://www.linkedin.com/in/mrjoshuawolfe

Platform Engineering Command Center:

https://gift3dmyndz.github.io/

---

# License

MIT License

Copyright © 2026 Joshua Wolfe.
