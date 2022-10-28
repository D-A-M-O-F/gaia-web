#include "HttpUpload.h"

void HttpUpload::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
  if ( req->getSession()->sessionId().empty() )
  {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("Session is required");
    resp->setStatusCode(drogon::k406NotAcceptable);
    callback(resp);
    return;                
  }

  drogon::MultiPartParser fileUpload;
  if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
  {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("Must only be one file");
    resp->setStatusCode(drogon::k403Forbidden);
    callback(resp);
    return;
  }

  auto &file = fileUpload.getFiles()[0];
  auto md5 = file.getMd5();
  auto resp = drogon::HttpResponse::newHttpResponse();

  if ( ( file.getFileExtension() == "cfg" ) || ( file.getFileExtension() == "zip" ) )
  {
    resp->setBody( "The server has calculated the file's MD5 hash to be " + md5);

    std::string _host_device = drogon::app().getCustomConfig()["session_root_path"].asString();
    std::string _session_root( _host_device + req->getSession()->sessionId() );

    if ( file.getFileExtension() == "cfg" )
    {
      file.saveAs( _session_root + "/cfg/gtfs.cfg" );
    }
    else if ( file.getFileExtension() == "zip" )
    {
      file.saveAs( _session_root + "/gtfs_in/input.zip" );
    }
  }
  else
  {
    resp->setBody("Only .cfg and .zip are accepted resources.");
    resp->setStatusCode(drogon::k415UnsupportedMediaType);
  }

  LOG_INFO << "The uploaded file has been saved to the ./uploads directory";
  callback(resp);
}
