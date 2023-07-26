/*******************************************************************************
 * Sketch name: AJAX code with WebSocket
 * Description: Additional code for extended Listing 7-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

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
