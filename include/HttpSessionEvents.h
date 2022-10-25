#pragma once

#include <drogon/SessionEvents.h>
#include <iostream>

using namespace drogon;


class HttpSessionEvents : public SessionEvents
{
public:
  void onSessionStart( const std::string& sessionId ) noexcept override
  { std::cout << "CREATE " << sessionId << std::endl; 
    create_environment(sessionId);
  }

  void onSessionDestroy( const std::string& sessionId ) noexcept override
  { std::cout << "DESTROY " << sessionId << std::endl;
    destroy_environment(sessionId); 
  }

private:
  void create_environment( const std::string& sessionId ) noexcept;
  void destroy_environment( const std::string& sessionId ) noexcept;


};


