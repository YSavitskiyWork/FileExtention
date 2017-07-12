function appendMessage(msg_json)
{
    // document.getElementById('size').innerHTML += text;
    // console.log(text)
    // var path = document.getElementById('path').value;
    var tbdy = document.getElementById("size_tb");
    tbdy.innerHTML = "";
    var tr = null;
    let i = 0;
    JSON.parse(msg_json, function(name, value)
    {
      if(name == "path")
        {
          document.getElementById("path").value = value;
        }

      if(name.includes("text"))
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + i;
        tr.addEventListener('click', changeDir);
        i += 1;
        var td = document.createElement('td');
        td.appendChild(document.createTextNode(value));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);
        
        //document.getElementById('size').innerHTML += "<tr><td class=\"mdl-list__item\">" + value + "</td>";
      }

      if (name.includes("value"))
      { 
        var td = document.createElement('td');
        td.appendChild(document.createTextNode(value));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);
        //document.getElementById('size').innerHTML += "<td class=\"mdl-list__item\">" + value + "</td></tr>";
        tbdy.appendChild(tr);
      }            
    });
}

function changeDir(event)
{
    var tr_id = null;
    if(event.target.tagName.toLowerCase() == 'td')
    {
      tr_id = event.target.parentElement.id;
    }
    else
      {
        tr_id = event.target.id;
      }
    var el = document.getElementById(tr_id);
    var attr = el.cells[0].textContent;
    var value = el.cells[1].textContent;
    if (value == "folder")
      document.getElementById("path").value += "/" + attr;
    sendNativeMessage();
}

function sendNativeMessage() 
{
  message = {"text": document.getElementById('path').value};
  console.log(message);
  port.postMessage(message);
}

function onNativeMessage(message) 
{
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
    port = chrome.runtime.connectNative(hostName);
    port.onMessage.addListener(onNativeMessage);
    port.onDisconnect.addListener(onDisconnected);
}

function documentEvents() 
{    
  document.getElementById('path').addEventListener('change', sendNativeMessage);
//   document.querySelector('body').addEventListener('click', function(event) {
//   if (event.target.tagName.toLowerCase() === 'li') {
//     // do your action on your 'li' or whatever it is you're listening for
//   }
// });

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