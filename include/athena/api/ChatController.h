#pragma once

#include <drogon/HttpController.h>

#include <memory>

#include "athena/core/AIEngine.h"

namespace athena::api
{

class ChatController final : public drogon::HttpController<ChatController>
{
   public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(ChatController::chat, "/api/v1/chat", drogon::Post);

    METHOD_LIST_END

    ChatController();

    void chat(const drogon::HttpRequestPtr& request,
              std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;

   private:
    std::shared_ptr<athena::core::AIEngine> engine_;
};

}  // namespace athena::api