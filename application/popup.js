function appendMessage(msg_json)
{
    var tbdy = document.getElementById("size_tb");
    tbdy.innerHTML = "";
    var tr = null;
    let ind = 0;
    JSON.parse(msg_json, function(name, value)
    {
      if(name == "parent")
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + ind;
        ind += 1;
        tr.addEventListener('click', openFile);
        var td = document.createElement('td');
        td.appendChild(document.createTextNode("back"));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);

        tbdy.appendChild(tr);
      }
      if(name == "file")
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + ind;
        ind += 1;
        var td = document.createElement('td');
        var pre = document.createElement('pre');
        pre.appendChild(document.createTextNode(value));
        td.appendChild(pre);
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);
        tbdy.appendChild(tr);
      }
      if(name == "path")
      {
        document.getElementById("path").value = value;
      }

      if(name.includes("text"))
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + ind;
        ind += 1;
        tr.addEventListener('click', openFile);
        var td = document.createElement('td');
        td.appendChild(document.createTextNode(value));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);
      }

      if (name.includes("value"))
      { 
        var td = document.createElement('td');
        td.appendChild(document.createTextNode(value));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);
        tbdy.appendChild(tr);
      }            
    });
}

function openFile(event)
{
    clearTimeout(timeout);
    timeout = setTimeout(function()
    {
      var tr_id = null;
      if(event.target.tagName.toLowerCase() == 'td')
      {
        tr_id = event.target.parentElement.id;
      }
      else if(event.target.tagName.toLowerCase() == 'tr')
        {
          tr_id = event.target.id;
        }
      var el = document.getElementById(tr_id);
      var attr = el.cells[0].textContent;
      
      if(attr == "back")
      {
        var path = document.getElementById("path").value;
        var arr = path.split("/");
        path = "";
        for(let i = 0; i<arr.length-1; i+=1)
        {
          path +="/" + arr[i];
        }
        document.getElementById("path").value = path;
      }
      else
      {
        var value = el.cells[1].textContent;
        document.getElementById("path").value += "/" + attr;
      }

      sendNativeMessage();
    }, 100);
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
};

var timeout = null;

var port = null;

connect();

document.addEventListener('DOMContentLoaded', documentEvents());