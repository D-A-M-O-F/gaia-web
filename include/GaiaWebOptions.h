#ifndef GAIA_WEB_OPTIONS_H
#define GAIA_WEB_OPTIONS_H

#include "config.h"
#include <string>

class GaiaWebOptions
{
private:
  /***/
  constexpr GaiaWebOptions();
  /***/
  constexpr ~GaiaWebOptions();

public:
  /***/
  static GaiaWebOptions& getInstance();

  /***/
  bool                   init( int argc, const char* argv[] );

  constexpr inline const std::string&   getRootPath() const noexcept
  { return m_sRootPath; }

  /***/
  constexpr inline const std::string&   getConfigFilename() const noexcept
  { return m_sCfgFileName; }

  /***/
  constexpr inline bool                 isInteractiveModeEnabled() const noexcept
  { return m_bEnableInteractiveMode; }

private:
  /***/
  bool                 init_priv( int argc, const char* argv[] ) noexcept; 

private:
  std::string     m_sRootPath;
  std::string     m_sCfgFileName;
  bool            m_bEnableInteractiveMode;
};


#endif // GAIA_WEB_OPTIONS_H