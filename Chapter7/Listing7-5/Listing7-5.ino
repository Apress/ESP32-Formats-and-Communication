/*******************************************************************************
 * Sketch name: HTML code for webpage with XML HTTP request
 * Description: HTML code for Listing 7-4
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html><head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<meta charset='UTF-8'>
</head>
<body>
<button onclick = 'update()'>LED</button>
<p><span id='LEDstate'>OFF</span></p>
<script>
function update()          // function called when button clicked
{
  var xhr = new XMLHttpRequest();     // XML HTTP request
  xhr.onreadystatechange = function()   // status of XML HTTP request
  {
    if (this.readyState == 4 && this.status == 200)
    document.getElementById('LEDstate').innerHTML = this.responseText;
  };                // identifier LEDstate updated
  xhr.open('GET', 'LEDurl', true);      // URL accessed
  xhr.send();           // transmit XML HTTP request
}
</script>
</body></html>
)";
