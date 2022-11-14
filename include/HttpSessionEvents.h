#pragma once

#include <iostream>

class HttpSessionEvents 
{
public:
  static void onSessionStart( const std::string& sessionId ) noexcept
  { create_environment(sessionId); }

  static void onSessionDestroy( const std::string& sessionId ) noexcept
  { destroy_environment(sessionId); }

public:
  static void create_environment( const std::string& sessionId ) noexcept;
  static void clean_environment( const std::string& sessionId ) noexcept;
  static void destroy_environment( const std::string& sessionId ) noexcept;

  static void clean_directory( const std::string& path, const std::string& ext ) noexcept;

};


