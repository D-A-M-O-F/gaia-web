#include "GaiaWebOptions.h"

#include "tclap/CmdLine.h"

using namespace TCLAP;
using namespace std;

class gaia_cmd_output : public StdOutput {
public:
    /***/
    virtual void version( [[maybe_unused]] CmdLineInterface &c) override {
        cout << "G.A.I.A. utility version " << c.getVersion() << endl;
        cout << "Copyright (C) 2022 Franceso Emanuele D'Agostino" << endl;
        cout << "This software is NOT free. For license information" << endl;
        cout << "please contact the owner at fedagostino@gmail.com ." << endl;
    }
};

constexpr GaiaWebOptions::GaiaWebOptions()
  : m_sCfgFileName( "gaia.cfg" ),
    m_bEnableInteractiveMode( false )
{}

constexpr GaiaWebOptions::~GaiaWebOptions()
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
  ValueArg<string> _configArg( "c", "config", 
                               "configuration file to be used. By default [config.json] in current directory will be used.", 
                               false, "config.json", "string");
  _cmd.add(_configArg);

  ValueArg<string> _rpathArg( "r", "rpath", 
                               "used to specify a common reference for all paths. Default value is empty [\"\"] so all paths in gaia.cfg can be both absolute or relative.", 
                               false, "./", "string");
  _cmd.add(_rpathArg);

  // Parse command line arguments
  _cmd.parse( argc, argv );
  
  m_sRootPath              = _rpathArg.getValue();
  m_sCfgFileName           = _configArg.getValue();

  return true;
}
