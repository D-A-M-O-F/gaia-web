#include "WebSocketLogger.h"
#include <iostream>
#include <unistd.h>

void WebSocketLogger::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
  // write your application logic here
  std::cout << "got new connection" << std::endl;

  std::cout << "Session ID " << req->session()->sessionId() << std::endl;
}

void WebSocketLogger::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
  if (type == WebSocketMessageType::Ping)
  {
    LOG_DEBUG << "recv a ping";
    wsConnPtr->send( "", 0, drogon::WebSocketMessageType::Pong );
  }
  else 
  if (type == WebSocketMessageType::Pong) 
  {
    LOG_DEBUG << "recv a pong";
  }
  else
  {
    wsConnPtr->send( "log\n\r", 5, drogon::WebSocketMessageType::Text );
  }    
}

void WebSocketLogger::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}

void WebSocketLogger::processing( const std::string& sessionid )
{
  std::string command;


  system( "docker run -it Image_name " );
}