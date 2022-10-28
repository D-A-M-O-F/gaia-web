
var webSocket = null;

function parseMessage( msg ){
  try {
    const obj = JSON.parse( msg );
    return obj;
  } catch ( e ) {
    return null;
  }
}

function sendMessage( cmd ){
  if ( webSocket ) {
    const json = {
      "v": "1.0",
      "c": cmd
    }
    const data = JSON.stringify( json );
    webSocket.send( data );
  }
}

function openWsConnection( protocol, hostname, port, endpoint ){
  var webSocketURL = protocol + "://" + hostname + ":" + port + endpoint;

  console.log("openWSConnection::Connecting to: " + webSocketURL);
  try {
    webSocket = new WebSocket(webSocketURL);

    webSocket.onopen = function(openEvent) {
        console.log("WebSocket OPEN: " + JSON.stringify(openEvent, null, 4));

        document.getElementById("start-processing").disabled = true;
    };

    webSocket.onclose = function (closeEvent) {
        console.log("WebSocket CLOSE: " + JSON.stringify(closeEvent, null, 4));

        document.getElementById("download-output").disabled = false;
    };

    webSocket.onerror = function (errorEvent) {
        console.log("WebSocket ERROR: " + JSON.stringify(errorEvent, null, 4));
    };

    webSocket.onmessage = function (messageEvent) {
      const wsMsg = parseMessage( messageEvent.data );
      
      if ( wsMsg === null ) {
        //@TODO
        return;
      }

      if ( wsMsg.v !== "1.0" ){
        //@TODO
        return;
      }

      if ( wsMsg.c === "begin" ){
        
        sendMessage("next");
        
      } else if ( wsMsg.c === "data" ){
        
        document.getElementById("logging").append( wsMsg.p );
          
        sendMessage("next");
      } else if ( wsMsg.c === "end" ){
        
        document.getElementById("download-output").addEventListener("click", function(){ location.href = wsMsg.p; } );
        
        webSocket.close();
      }
    };
  } catch (exception) {
    console.error(exception);
  }  
}

