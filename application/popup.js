function appendMessage(msg_json)
{
    // document.getElementById('size').innerHTML += text;
    // console.log(text)
    // var path = document.getElementById('path').value;
    document.getElementById("size").innerHTML = "";
    JSON.parse(msg_json, function(name, value)
    {
      if(name == "text")
      {
        document.getElementById('size').innerHTML += value;
        return;
      }

      if (name != "")
      {
        document.getElementById('size').innerHTML += "<li class=\"mdl-list__item\">" + value + "</li>";
      }      
    });
}

function sendNativeMessage() 
{
  message = {"text": document.getElementById('path').value};
  console.log(message);
  port.postMessage(message);
  //appendMessage("Sent message: <b>" + JSON.stringify(message) + "</b>");
}

function onNativeMessage(message) 
{
  //appendMessage("Received message: <b>" + JSON.stringify(message) + "</b>");
  appendMessage(JSON.stringify(message));
}

function onDisconnected() 
{
  appendMessage("Failed to connect: " + chrome.runtime.lastError.message);
  port = null;
}

function connect() 
{
    var hostName = "com.ys.my_nmh";
    //appendMessage("Connecting to native messaging host <b>" + hostName + "</b>")
    port = chrome.runtime.connectNative(hostName);
    port.onMessage.addListener(onNativeMessage);
    port.onDisconnect.addListener(onDisconnected);
}

function documentEvents() 
{    
  document.getElementById('path').addEventListener('change', sendNativeMessage);
};

var port = null;

connect();

document.addEventListener('DOMContentLoaded', documentEvents());




// function getSize(port)
// {
//     var path = document.getElementById('path').value;
//     document.getElementById("size").innerHTML = path;

//     chrome.extension.sendRequest(path);
// }

// chrome.extension.onRequest.addListener(function(data, sender) {
//     if (data.length > 0) {
//         connect();
//         sendNativeMessage(data);
//     }
// });

// function documentEvents() 
// {    
//   document.getElementById('path').addEventListener('change', 
//     function() {getSize()});
// };