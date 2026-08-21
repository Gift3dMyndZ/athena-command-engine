# Athena Architecture

## Initial Request Flow

```text
User
 |
 v
React Command Center
 |
 v
Athena REST API
 |
 v
Request Validation
 |
 v
AI Engine
 |
 v
AI Provider Interface
 |
 v
Local llama.cpp Server
 |
 v
JSON Response
```

## Architectural Goals

1. Keep HTTP controllers separate from AI business logic.
2. Use dependency injection to make components testable.
3. Return consistent JSON error responses.
4. Use request IDs for troubleshooting.
5. Keep local AI model files outside Git.
6. Add health, readiness, and metrics endpoints.
7. Support future GitHub and resume intelligence agents.