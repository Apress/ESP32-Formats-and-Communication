/*******************************************************************************
 * Sketch name: XML HTTP requests with two LEDs
 * Description: HTML code for Listing 7-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

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
<body>
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
function update(butn)        // function called when button clicked
{
  var URL, state;
  if(butn == 'LEDG')          // green LED button clicked
  {
    URL = 'LEDGurl';          // XMLHTTP request location
    state = 'LEDGstate';        // green LED identifier
  }
  else if(butn == 'LEDR')
  {
    URL = 'LEDRurl';
    state = 'LEDRstate';
  }
  var xhr = new XMLHttpRequest();     // XML HTTP request
  xhr.onreadystatechange = function()   // status of XML HTTP
  {
    if (this.readyState == 4 && this.status == 200)   // server response
    document.getElementById(state).innerHTML = this.responseText;
  };                // identifier state updated
  xhr.open('GET', URL, true);       // URL accessed
  xhr.send();           // transmit XML HTTP request
}
</script>
</body></html>
)";
