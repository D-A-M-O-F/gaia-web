#include "HttpAlive.h"
#include "config.h"
#include "GaiaWebStats.h"

void HttpAlive::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
  std::string alive   = "<b>Alive!<b><br><br>";
  std::string version = "Version: " make_version(GAIA_WEB_VERSION_MAJOR,GAIA_WEB_VERSION_MINOR,GAIA_WEB_VERSION_PATCH) "<br><br>";
  std::string stats   = format( "<table><tr><td><b>Configuration uploads:<b></td><td> %d </td></tr>"
                                       "<tr><td><b>GTFS uploads         :<b></td><td> %d </td></tr>"
                                       "<tr><td><b>Processing requests  :<b></td><td> %d </td></tr></table><br>", 
                                 GaiaWebStats::_cfg_uploads.load(),
                                 GaiaWebStats::_zip_uploads.load(),
                                 GaiaWebStats::_processing_requests.load()
                              );

  auto resp = drogon::HttpResponse::newHttpResponse();
  resp->setBody( alive + version + stats );
  resp->setStatusCode(drogon::k200OK);
  callback(resp);
}
