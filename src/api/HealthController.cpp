#include "athena/api/HealthController.h"

namespace athena::api
{

void HealthController::health(const drogon::HttpRequestPtr&,
                              std::function<void(const drogon::HttpResponsePtr&)>&& callback) const
{
    Json::Value body;
    body["status"] = "healthy";
    body["service"] = "athena-command-engine";
    body["version"] = "0.1.0";

    auto response = drogon::HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(drogon::k200OK);

    callback(response);
}

}  // namespace athena::api