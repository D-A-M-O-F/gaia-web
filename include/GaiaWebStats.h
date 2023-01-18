#ifndef GAIA_WEB_STATS_H
#define GAIA_WEB_STATS_H

#include "GaiaWebDefines.h"
#include "config.h"
#include <atomic>
#include <vector>

struct GaiaWebStats
{
  static std::atomic<uint32_t>   _cfg_uploads;
  static std::atomic<uint32_t>   _zip_uploads;
  static std::atomic<uint32_t>   _processing_requests;
};

/**
 * @brief Waiting std::format to be fully supported.
 */
template<typename ... Args>
static constexpr std::string format( const char* format, Args ... args ) noexcept
{
  int buffer_size = std::snprintf( nullptr, 0, format, args ... );
  if ( buffer_size <= 0 )
    return std::string();

  std::vector<char> buffer(buffer_size + 1);        
  std::snprintf(&buffer[0], buffer.size(), format, args ... );
  
  return std::string( buffer.begin(), buffer.end()-1 );
}

#endif // GAIA_WEB_STATS_H