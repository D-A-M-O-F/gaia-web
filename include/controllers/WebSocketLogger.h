#pragma once

#include <drogon/WebSocketController.h>
#include <fstream>

using namespace drogon;

class WebSocketLogger : public drogon::WebSocketController<WebSocketLogger>
{
public:
  /***/
  void handleNewMessage(const WebSocketConnectionPtr&, std::string &&, const WebSocketMessageType &) override;
  /***/
  void handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr&) override;
  /***/
  void handleConnectionClosed(const WebSocketConnectionPtr&) override;

  WS_PATH_LIST_BEGIN
  // list path definitions here;
  WS_PATH_ADD("/output" );
  WS_PATH_LIST_END

private:
  using session_id_t = std::string;
  struct session_status_t {
    session_status_t()
      : served(false), bufdata(0)
    {}

    bool is_ready() const
    { return result.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

    session_id_t      session_id;
    std::future<int>  result;

    bool              served;
    char              buffer[1024];   // Buffer used to store log information
    std::streamsize   bufdata;        // Valid buffer data
    std::ifstream     logfile;
  };

  using sessions_status_map_t = std::unordered_map<std::string,std::shared_ptr<session_status_t>>;

private:
  static void send( const WebSocketConnectionPtr& wsConnPtr,
                            const std::string& version,
                            const std::string& command,
                            const std::string& status,
                            const std::string& payload
                          );
  static int  processing( const std::string& sessionid ); 
  
  static std::string load_log( session_status_t& status ); 

private:
  sessions_status_map_t   m_mapSessions;

};
