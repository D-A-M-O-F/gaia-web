#include "HttpUpload.h"

void HttpUpload::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
  std::cout << req->getSession()->sessionId() << std::endl;
  
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
  resp->setBody( "The server has calculated the file's MD5 hash to be " + md5);
  file.save();
  LOG_INFO << "The uploaded file has been saved to the ./uploads directory";
  callback(resp);
}
