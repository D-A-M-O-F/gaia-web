#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class HttpAlive : public drogon::HttpSimpleController<HttpAlive>
{
public:
  void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
  PATH_LIST_BEGIN
  // list path definitions here;
    PATH_ADD("/alive", {drogon::Get} );
  PATH_LIST_END
};
