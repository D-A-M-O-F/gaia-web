#include "HttpSessionEvents.h"
#include "GaiaWebOptions.h"
#include <drogon/drogon.h>
#include <filesystem>
#include <iostream>


using namespace drogon;

void HttpSessionEvents::create_environment( const std::string& sessionId ) noexcept
{
  LOG_DEBUG << "CREATE ENVIRONMENT FOR SESSION ID = " << sessionId;

  std::string _host_device = drogon::app().getCustomConfig()["session_root_path"].asString();

  std::filesystem::path _session_root( _host_device + sessionId );
  std::filesystem::create_directory( _session_root );
  std::filesystem::create_directory( _session_root / GTFS_INPUT  );
  std::filesystem::create_directory( _session_root / GTFS_OUTPUT );
  std::filesystem::create_directory( _session_root / GTFS_LOG    );
  std::filesystem::create_directory( _session_root / GTFS_CFG    );
  std::filesystem::create_directory( _session_root / GTFS_RCFG   );

  std::error_code ec( 0, std::system_category() );

  if ( std::filesystem::copy_file( GaiaWebOptions::getInstance().getGaiaHomePath() + GTFS_RCFG + "/gaia.cfg", 
                                    _session_root / GTFS_RCFG / "gaia.cfg",
                                    std::filesystem::copy_options::overwrite_existing,
                                    ec
                                  ) == false )
  {
    LOG_DEBUG << "Failed to copy gaia.cfg";
    //@TODO
  }

  ec.clear();
  if ( std::filesystem::copy_file( GaiaWebOptions::getInstance().getGaiaHomePath() + GTFS_CFG + "/gtfs.cfg"  , 
                                    _session_root / GTFS_CFG / "gtfs.cfg",
                                    std::filesystem::copy_options::overwrite_existing,
                                    ec
                                  ) == false )
  {
    LOG_DEBUG << "Failed to copy gtfs.cfg "; 
    //@TODO
  }
  
}

void HttpSessionEvents::clean_environment( const std::string& sessionId ) noexcept
{
  LOG_DEBUG << "CLEAN ENVIRONMENT FOR SESSION ID = " << sessionId;

  std::string           _host_device = drogon::app().getCustomConfig()["session_root_path"].asString();
  std::filesystem::path _session_root( _host_device + sessionId );

  std::filesystem::path _log_path   ( _session_root / GTFS_LOG    );
  std::filesystem::path _input_path ( _session_root / GTFS_INPUT  );
  std::filesystem::path _output_path( _session_root / GTFS_OUTPUT );

  clean_directory(_log_path   ,".log");
  clean_directory(_input_path ,".txt");
  clean_directory(_output_path,".txt");
}

void HttpSessionEvents::destroy_environment( const std::string& sessionId ) noexcept
{
  LOG_DEBUG << "DESTROY ENVIRONMENT FOR SESSION ID = " << sessionId;

  std::string _host_device = drogon::app().getCustomConfig()["session_root_path"].asString();

  std::filesystem::path _session_root( _host_device + sessionId );

  std::error_code ec( 0, std::system_category() );
  if ( std::filesystem::remove_all( _session_root, ec ) == false )
  {
    //@TODO
  }
}

void HttpSessionEvents::clean_directory( const std::string& path, const std::string& ext ) noexcept
{
  for (auto const& dir_entry : std::filesystem::directory_iterator{path}) 
  {
    if ( dir_entry.path().extension() == ext )
    {
      std::error_code ec( 0, std::system_category() );
      std::filesystem::remove( dir_entry.path(), ec );
    }
  }
}



