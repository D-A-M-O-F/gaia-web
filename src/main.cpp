#include "GaiaWebOptions.h"
#include <drogon/drogon.h>
#include <HttpSessionEvents.h>


int main( [[maybe_unused]] int argc, [[maybe_unused]] const char* argv[] ) {
  // Initialize command line options
  GaiaWebOptions::getInstance().init( argc, argv );

  drogon::app().setSessionEventsHandler( std::make_unique<HttpSessionEvents>() );
  drogon::app().setDocumentRoot( GaiaWebOptions::getInstance().getWebRootPath() );

  //Load config file
  drogon::app().loadConfigFile( GaiaWebOptions::getInstance().getConfigFilename() );

  //Run HTTP framework,the method will block in the internal event loop
  drogon::app().run();
  return 0;
}
