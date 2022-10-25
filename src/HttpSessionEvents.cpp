#pragma once

#include "HttpSessionEvents.h"
#include <filesystem>
#include <iostream>


using namespace drogon;

#define HOST_DEVICE "/dev/shm/"

void HttpSessionEvents::create_environment( const std::string& sessionId ) noexcept
{
  std::filesystem::path _session_root( HOST_DEVICE + sessionId );
  std::filesystem::create_directory( _session_root );
  std::filesystem::create_directory( _session_root / "gtfs_input"  );
  std::filesystem::create_directory( _session_root / "gtfs_output" );
  std::filesystem::create_directory( _session_root / "log"         );
  std::filesystem::create_directory( _session_root / "cfg"         );
}

void HttpSessionEvents::destroy_environment( const std::string& sessionId ) noexcept
{
  std::filesystem::path _session_root( HOST_DEVICE + sessionId );
  std::filesystem::remove_all( _session_root );
}





