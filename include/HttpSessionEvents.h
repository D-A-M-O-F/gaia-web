#pragma once

#include <drogon/SessionEvents.h>
#include <iostream>

using namespace drogon;


class HttpSessionEvents : public SessionEvents
{
public:
  void onSessionStart( const std::string& sessionId ) noexcept override
  { create_environment(sessionId); }

  void onSessionDestroy( const std::string& sessionId ) noexcept override
  { destroy_environment(sessionId); }

public:
  static void create_environment( const std::string& sessionId ) noexcept;
  static void clean_environment( const std::string& sessionId ) noexcept;
  static void destroy_environment( const std::string& sessionId ) noexcept;

  static void clean_directory( const std::string& path, const std::string& ext ) noexcept;


};


