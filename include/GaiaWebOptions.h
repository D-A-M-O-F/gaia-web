#ifndef GAIA_WEB_OPTIONS_H
#define GAIA_WEB_OPTIONS_H

#include "GaiaWebDefines.h"
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

  /***/
  constexpr inline const std::string&   getWebRootPath() const noexcept
  { return m_sWebRootPath; }

  /***/
  constexpr inline const std::string&   getWebConfigFilename() const noexcept
  { return m_sWebCfgFileName; }

  /***/
  constexpr inline const std::string&   getGaiaHomePath() const noexcept
  { return m_sGaiaHomePath; }

private:
  /***/
  bool                 init_priv( int argc, const char* argv[] ) noexcept; 

private:
  std::string     m_sWebRootPath;
  std::string     m_sWebCfgFileName;
  std::string     m_sGaiaHomePath;
};

#endif // GAIA_WEB_OPTIONS_H