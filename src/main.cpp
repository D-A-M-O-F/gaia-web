#include "GaiaWebOptions.h"
#include <drogon/drogon.h>
#include <HttpSessionEvents.h>


int main( [[maybe_unused]] int argc, [[maybe_unused]] const char* argv[] ) {
  // Initialize command line options
  GaiaWebOptions::getInstance().init( argc, argv );

  drogon::app().registerSessionStartAdvice ( &HttpSessionEvents::onSessionStart );
  drogon::app().registerSessionDestroyAdvice( &HttpSessionEvents::onSessionDestroy );

  drogon::app().setDocumentRoot( GaiaWebOptions::getInstance().getWebRootPath() );

  drogon::app().setCustom404Page( 
                                  drogon::HttpResponse::newFileResponse( drogon::app().getDocumentRoot() + "404.html" ) 
                                );

  //Load config file
  drogon::app().loadConfigFile( GaiaWebOptions::getInstance().getWebConfigFilename() );
  
  //Run HTTP framework,the method will block in the internal event loop
  drogon::app().run();
  return 0;
}
