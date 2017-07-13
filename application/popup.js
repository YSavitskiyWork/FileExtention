var timeout = setTimeout(function() {}, 100);

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
      if(name == "parent")
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + i;
        tr.addEventListener('click', openFile);
        i += 1;
        var td = document.createElement('td');
        td.appendChild(document.createTextNode("back"));
        td.className = "mdl-data-table__cell--non-numeric";
        tr.appendChild(td);

        tbdy.appendChild(tr);
      }
      if(name == "file")
      {
        tr = document.createElement('tr');
        tr.id = "size_tr_" + i;
        i += 1;
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
        tr.id = "size_tr_" + i;
        tr.addEventListener('click', openFile);
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
      else
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

var port = null;

connect();

document.addEventListener('DOMContentLoaded', documentEvents());