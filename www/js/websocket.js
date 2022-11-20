
var webSocket   = null;
var bytesTotal  = 0;
var bytesCount  = 0;
var waitingTime = 0;
var bytesLimit  = 100000;

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

function ackBegin( live ){
  if ( webSocket ) {
    const json = {
      "v": "1.0",
      "c": "abegin",
      "p": live
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
        document.getElementById('logStreamEnabled').disabled = true;
    };

    webSocket.onclose = function (closeEvent) {
        console.log("WebSocket CLOSE: " + JSON.stringify(closeEvent, null, 4));

        document.getElementById("download-output").disabled = false;
        document.getElementById('logStreamEnabled').disabled = false;

        notifyInfo( "Operation completed!", true );
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

        bytesCount  = 0;
        bytesTotal  = 0;
        waitingTime = 0;
        document.getElementById("logging").innerHTML = "";
        
        ackBegin( document.getElementById('logStreamEnabled').checked );
        
      } else if ( wsMsg.c === "data" ){

        if ( bytesCount > bytesLimit )
        {
          bytesCount = 0;
          document.getElementById("logging").innerHTML = "";
        }

        bytesCount += wsMsg.p.length;
        bytesTotal += wsMsg.p.length;

        if ( wsMsg.s === "busy" )
        {
          waitingTime += 1;

          if ( waitingTime === 1 )
          { document.getElementById("logging").innerHTML = waitingTime + " second"; }
          else
          { document.getElementById("logging").innerHTML = waitingTime + " seconds"; }

          setTimeout(() => { sendMessage("next"); }, 1000);
        }
        else
        {
          document.getElementById("logging").append( wsMsg.p );
          document.getElementById('logging').scrollTop = document.getElementById('logging').scrollHeight;
          
          sendMessage("next");
        }

      } else if ( wsMsg.c === "end" ){
        
        document.getElementById("download-output").addEventListener("click", function(){ location.href = wsMsg.p; } );

        if ( wsMsg.s !== "ok" )
        {
          document.getElementById("logging").append( "\n\n----------\nAn ERROR occurred.\nDownload output for details.\n----------" );
          document.getElementById('logging').scrollTop = document.getElementById('logging').scrollHeight;
        }
        
        webSocket.close();
      }
    };
  } catch (exception) {
    console.error(exception);
  }  
}

