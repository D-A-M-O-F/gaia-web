#include "HttpAlive.h"
#include "config.h"

void HttpAlive::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
  auto resp = drogon::HttpResponse::newHttpResponse();
  resp->setBody("Alive!\nVersion: " make_version(GAIA_WEB_VERSION_MAJOR,GAIA_WEB_VERSION_MINOR,GAIA_WEB_VERSION_PATCH) );
  resp->setStatusCode(drogon::k200OK);
  callback(resp);
}
