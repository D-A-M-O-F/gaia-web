#include "WebSocketLogger.h"
#include "HttpSessionEvents.h"
#include "GaiaWebOptions.h"
#include <filesystem>
#include <iostream>
#include <unistd.h>

/*
PROTOCOL

"v" version 
    accepted value "1.0" 
"c" command
    "data"     raw text data in "p"
    "begin"    start processing
    "next"     requesting next buffer/command
    "end"      namefile to download in "p"

"s" status
    "ok"      
    "<error>"  error code  

"p" payload
    arbitrary  amount of data

*/

void WebSocketLogger::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
  session_id_t idSession = req->session()->sessionId();
  if ( idSession.empty() == true )
  {
    //@TODO
    return;
  }

  if ( m_mapSessions.contains( idSession ) == true )
  {
    if ( wsConnPtr->hasContext() == true )
    {
      LOG_DEBUG << "A SESSION ALREADY PRESENT AND IN PROGRESS";
      wsConnPtr->shutdown();
      return;
    }

    HttpSessionEvents::clean_environment(idSession);
  }

  m_mapSessions[idSession] = std::make_shared<session_status_t>();

  m_mapSessions[idSession]->session_id  = idSession;
  m_mapSessions[idSession]->result      = std::async( processing, idSession );

  wsConnPtr->setContext( m_mapSessions[idSession] );

  send( wsConnPtr, "1.0", "begin", "ok", idSession );
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
  if (type == WebSocketMessageType::Text) 
  {
    Json::Value  in_json;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse( message, in_json );

    if ( !parsingSuccessful )
    {
      LOG_DEBUG << "Error parsing the ws message";
    }

    if ( in_json["v"].asString() != "1.0" )
    {
      return;
    }

    if ( in_json["c"].asString() == "next" )
    {
      session_status_t& status = wsConnPtr->getContextRef<session_status_t>();

      if ( ( status.is_ready() == false ) || (status.logfile.is_open()==true) )
      {
        const std::string payload = load_log( status );

        send( wsConnPtr, "1.0", "data", "ok",
              payload
            );
      }
      else
      {
        std::string retVal = "ok";
        if ( status.result.get() != 0 )
        {
          retVal = std::to_string( status.result.get() );
        }

        LOG_DEBUG << "END PROCESSING:" << status.session_id;

        send( wsConnPtr, "1.0", "end", retVal,
              WWW_PROCESSING"/" + status.session_id + "/output.zip" 
            );
      }
    }
      
  }    
}

void WebSocketLogger::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
  if ( wsConnPtr->hasContext() )
  {
    std::string sessionid = wsConnPtr->getContextRef<session_status_t>().session_id;
    
    HttpSessionEvents::clean_environment(sessionid);
    
    m_mapSessions.erase(sessionid);
  }
}

void WebSocketLogger::send( const WebSocketConnectionPtr& wsConnPtr,
                            const std::string& version,
                            const std::string& command,
                            const std::string& status,
                            const std::string& payload
                          )
{
  Json::Value               out_json;
  Json::StreamWriterBuilder builder;  

  out_json["v"] = version;;
  out_json["c"] = command;
  out_json["s"] = status;
  out_json["p"] = payload;

  const std::string out_data = Json::writeString(builder, out_json);

  wsConnPtr->send( out_data, drogon::WebSocketMessageType::Text );
}

int WebSocketLogger::processing( const std::string& sessionid )
{
  std::string command = GaiaWebOptions::getInstance().getGaiaHomePath() + GAIA_SCRIPTS +
                        "/processing.sh " + sessionid + " " +
                        GaiaWebOptions::getInstance().getGaiaHomePath() + GAIA_BIN + "/";

  LOG_DEBUG << "START PROCESSING:" << command;

  return system( command.c_str() );
}

std::string WebSocketLogger::load_log( session_status_t& status )
{
  if ( ( status.logfile.is_open() == false ) && ( status.served == false ) )
  {
    std::string _host_device = drogon::app().getCustomConfig()["session_root_path"].asString();

    std::filesystem::path _session_root( _host_device + status.session_id );
    std::filesystem::path _log_path( _session_root / GTFS_LOG  );

    std::string           _log_filename;
    for (auto const& dir_entry : std::filesystem::directory_iterator{_log_path}) 
    {
      _log_filename = dir_entry.path();
      break;
    }

    if ( _log_filename.empty() == false )
    {
      status.served == true;
      status.logfile.open(_log_filename);
    }
  }

  if ( status.logfile.is_open() == true )
  {
    status.bufdata = status.logfile.readsome( status.buffer, sizeof(status.buffer) );
    if ( status.is_ready() && (status.bufdata == 0) )
    {
      status.logfile.close();
    }
  }
  
  return std::string( status.buffer, status.bufdata );
} 
