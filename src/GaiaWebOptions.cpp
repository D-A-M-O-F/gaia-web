#include "GaiaWebOptions.h"

#include "tclap/CmdLine.h"

using namespace TCLAP;
using namespace std;

class gaia_cmd_output : public StdOutput {
public:
    /***/
    virtual void version( [[maybe_unused]] CmdLineInterface &c) override {
        cout << "G.A.I.A. WEB version " << c.getVersion() << endl;
        cout << "Copyright (C) 2022 D.A.M.O.F." << endl;
        cout << "email: info@damof.org." << endl;
    }
};

GaiaWebOptions::GaiaWebOptions()
  : m_sWebRootPath( "./www" ),
    m_sWebCfgFileName( "config.json" ),
    m_sGaiaHomePath( "./" )
{}

GaiaWebOptions::~GaiaWebOptions()
{}

GaiaWebOptions& GaiaWebOptions::getInstance()
{
  static GaiaWebOptions  _instance = GaiaWebOptions();

  return _instance;
}

bool GaiaWebOptions::init( int argc, const char* argv[] )
{
  bool _bRetValue = false;

  try {
    _bRetValue = init_priv( argc, argv );
  }
  catch ( ArgException &e )
  {
  }

  return _bRetValue;
}
  
bool GaiaWebOptions::init_priv( int argc, const char* argv[] ) noexcept
{
  gaia_cmd_output _cmd_output;
  CmdLine         _cmd( "G.A.I.A. Web Server", ' ', make_version(GAIA_WEB_VERSION_MAJOR,GAIA_WEB_VERSION_MINOR,GAIA_WEB_VERSION_PATCH) );
  _cmd.setOutput(&_cmd_output);

  // Define a value argument and add it to the command line.
  ValueArg<string> _webConfigArg( "c", "config", 
                               "configuration file to be used. By default [config.json] in current directory will be used.", 
                               false, "config.json", "string");
  _cmd.add(_webConfigArg);

  ValueArg<string> _webRpathArg( "r", "rpath", 
                               "used to specify web root path. Default value is [\"./www\"]. Note: \"document_root\" in config.json override this value, that means that if you want parameter to be applied your configuration fille should not contain this value.", 
                               false, "./www", "string");
  _cmd.add(_webRpathArg);

  ValueArg<string> _gaiaHomeArg( "g", "gaia_home", 
                               "used to specify gaia home path. Default value is [\"./\"].", 
                               false, "./", "string");
  _cmd.add(_gaiaHomeArg);

  // Parse command line arguments
  _cmd.parse( argc, argv );
  
  m_sWebRootPath           = _webRpathArg.getValue();
    if (m_sWebRootPath.back() != '/')
  { m_sWebRootPath.append("/"); } 

  m_sWebCfgFileName        = _webConfigArg.getValue();

  m_sGaiaHomePath          = _gaiaHomeArg.getValue(); 
  if (m_sGaiaHomePath.back() != '/')
  { m_sGaiaHomePath.append("/"); } 

  return true;
}
