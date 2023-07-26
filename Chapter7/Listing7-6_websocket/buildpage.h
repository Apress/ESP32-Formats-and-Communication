char page[] PROGMEM = R"(
<!DOCTYPE html><html><head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<meta charset='UTF-8'>
<title>QR codes</title>
<style>
body {margin-top:50px; font-family:Arial; text-align:center}
.btn {display:block; width:280px; margin:auto; padding:30px;
      font-size:30px; color:black; text-decoration:none}
.gn {background-color:DarkSeaGreen}
.rd {background-color:Thistle}
td {font-size:30px; text-align:center}
</style></head>
<body id='initialise'>
<h1>QR codes</h1>
<center>
<table style='width:50%'>
<tr><td><button class = 'btn gn' id='LEDG'
         onclick = 'update(id)'>Green LED</button></td>
    <td><button class = 'btn rd' id='LEDR'
         onclick = 'update(id)'>Red LED</button></td></tr>
<tr><td><span id='LEDGstate'>OFF</span></td>
    <td><span id='LEDRstate'>OFF</span></td></tr>
</table>
</center>
<script>
var wskt;
document.getElementById('initialise').onload = function()
{init()};

function init()
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
  wskt.onmessage = function(rx)
  {
    var obj = JSON.parse(rx.data);
    document.getElementById('LEDGstate').innerHTML = obj.var1;
    document.getElementById('LEDRstate').innerHTML = obj.var2;
  };
}

function update(butn)
{
  wskt.send(butn);
}
</script>
</body></html>
)";
