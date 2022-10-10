
var webSocket = null;

function openWsConnection( protocol, hostname, port, endpoint ){
  var webSocketURL = protocol + "://" + hostname + ":" + port + endpoint;

  console.log("openWSConnection::Connecting to: " + webSocketURL);
  try {
      webSocket = new WebSocket(webSocketURL);

      webSocket.onopen = function(openEvent) {
          console.log("WebSocket OPEN: " + JSON.stringify(openEvent, null, 4));
          /*
          document.getElementById("btnSend").disabled       = false;
          document.getElementById("btnConnect").disabled    = true;
          document.getElementById("btnDisconnect").disabled = false;*/
          webSocket.send("{msg_type=next_chunk}");
      };

      webSocket.onclose = function (closeEvent) {
          console.log("WebSocket CLOSE: " + JSON.stringify(closeEvent, null, 4));
          /*
          document.getElementById("btnSend").disabled       = true;
          document.getElementById("btnConnect").disabled    = false;
          document.getElementById("btnDisconnect").disabled = true;
          */
      };

      webSocket.onerror = function (errorEvent) {
          console.log("WebSocket ERROR: " + JSON.stringify(errorEvent, null, 4));
      };

      webSocket.onmessage = function (messageEvent) {
          var wsMsg = messageEvent.data;
          console.log("WebSocket MESSAGE: " + wsMsg);
          if (wsMsg.indexOf("error") > 0) {
              document.getElementById("logging").value += "error: " + wsMsg.error + "\r\n";
          } else {
              document.getElementById("logging").innerText += wsMsg;
              
              //webSocket.send("{msg_type=next_chunk}");
          }
      };
  } catch (exception) {
      console.error(exception);
  }  
}

