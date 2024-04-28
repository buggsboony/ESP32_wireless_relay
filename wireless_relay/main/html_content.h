/* File used to set HTML content */
#include <iostream>
#include <map>

using namespace std;

//2023-08-16 17:04:13 - Paste here HTML content
string HTML_CONTENT = R"(


<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{{TITLE}}</title>

    <style>
        .info
        {
            font-size: 1.0em;
        }

    body
    {
        color:antiquewhite;
        background-color: #404040;
    }

    textarea
    {
        width: 80%;
        height:600px;
        border: solid 1px gainsboro;
        background: #100e00;
        color: #aeff2c;
        font-size: 1.1em;
        font-family: monospace;
    }

    #response
    {
        width: 80%;
        height:200px;
        border: solid 1px gainsboro;
        background: #c9c9c5;
        color: #0d363f;
        font-size: 1.1em;
        font-family: monospace;
    }

    
    .maxi_button
    {
        margin: 10px;
        min-height: 223px;
        min-width: 320px;
        padding: 8px;
                background: #eff340;
    color: #1765c9;
        border: solid 1px #141504;
        font-size: 10.31em;
        font-family: monospace;
    }

    button
    {
        margin: 10px;
        min-height: 60px;
        min-width: 120px;
        padding: 8px;       
        
        border:solid 1px #141504;
        color: #141504;
        font-size: 1.31em;
        font-family: monospace;
    }
/*     
    button:hover {
  background-color: bisque;
} */
    form{
        display: flex;
        align-content: stretch;
        justify-content: center;
        flex-direction: column;
        flex-wrap: wrap;
        align-items: center;
    }

    .btn-primary
    {
            background: #eff340;
            color: #1765c9;
    }
    .btn-danger
    {
        background:#b17b7d;
    }
    .buttons
    {
        display: flex;
        flex-direction: row;
        flex-wrap: nowrap;
        justify-content: space-between;
        align-content: center;
        align-items: flex-end;
    }

    </style>


<script>

           //2023-08-17 16:58:26 - Send command to server
        function sendCommand(command_name, value, onEnd)
        {
                var url = "/relay";                
                var payload = value;
                //2023-08-17 18:26:22 - response callback
                var onResponse= function(data)
                {
                    var area = document.getElementById("response");
                    area.value = data;
                    console.log(data);                    
                }      
                var postData = new FormData();	
                postData.append(command_name, payload);
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        onResponse( xhttp.responseText );                 
                    }
                    //2023-10-22 22:02:13
                    if(onEnd)
                    {
                        onEnd(xhttp.responseText);
                    }
                };          
            xhttp.open("POST", url, true);
            xhttp.send(postData);
        }//sendData, sendCommand


        function saveConfig()
        {                
                var area = document.getElementById("postdata");
                sendCommand("data",area.value);
        }//saveConfig

        function turnOn(sender)
        {                              
            var com = "GO";
            if(sender)
            {                
                sender.innerText="⏳";
                //sender.style.background="gold";
            }
                sendCommand("data",com,function(){
                    setTimeout(() => {
                           sender.innerText=com;
                           //sender.style.background="#33b5af";
                    }, 700);             
            });
        }//saveConfig


        function turnOff(sender)
        {               
            var com = "OFF";
            if(sender)
            {                
                sender.innerText="⏳";
                //sender.style.background="gold";
            }
                sendCommand("data",com,function(){
                    setTimeout(() => {
                           sender.innerText=com;
                           //sender.style.background="#33b5af";
                    }, 700);             
            });
        }//saveConfig


        //2023-08-30 15:22:29 - Restart command, data is local date time
        function restart()
        {           
                var area = document.getElementById("postdata");
                var when = new Date();                
                sendCommand("restart",when.toISOString().substring(0,10)+" "+when.toLocaleTimeString() );                
        }//saveConfig


        function loadSample(sender)
        {
            if( confirm("Load default sample?") )
            {
                var str = JSON.stringify(espeed_alert_sample, null,4);
                var textarea = document.getElementById("postdata");
                textarea.value = str;
            }
        }//loadSample
     
    </script>


</head>

<body>    
    <h4 onclick="loadSample(this);">{{TITLE}}</h4>    
    <section action="{{POST_URI}}" method="post">                   
        <div class="buttons">
            <button class="btn-primary" onclick="turnOn();">GO</button>
            <button class="btn-danger" onclick="restart(this);">Restart</button>
        </div> 
         
            <textarea id="response">response here</textarea> <br/>
       <div class="buttons">
        <button class="btn-primary maxi_button" onclick="turnOn(this);">GO</button>        
       </div>
    </section>

   

</body>
</html>















)";//2023-08-16 17:01:03 - End of literal strign