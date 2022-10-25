#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class HttpUpload : public drogon::HttpSimpleController<HttpUpload>
{
public:
  void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
  PATH_LIST_BEGIN
  // list path definitions here;
    PATH_ADD("/upload_endpoint", {drogon::Post} );
  PATH_LIST_END
};
