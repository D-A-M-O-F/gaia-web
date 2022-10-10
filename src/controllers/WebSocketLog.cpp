#include "WebSocketLog.h"
#include <iostream>

void WebSocketLog::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
    std::cout << "got new connection" << std::endl;
}

void WebSocketLog::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
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

void WebSocketLog::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}
