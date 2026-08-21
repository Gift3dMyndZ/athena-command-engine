#pragma once

#include <drogon/HttpController.h>

namespace athena::api
{

class HealthController final : public drogon::HttpController<HealthController>
{
   public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(HealthController::health, "/api/v1/health", drogon::Get);

    METHOD_LIST_END

    void health(const drogon::HttpRequestPtr& request,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
};

}  // namespace athena::api