/*******************************************************************************
 * Sketch name: HTML code for webpage when no internet
 * Description: HTML code for Listing 10-11
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<body>
<img id='bulb' onclick='switchImage()' src='off' width='100' height='180'>
<p>Click the bulb to turn on or off</p>
<script>
function switchImage()
{
  var image = document.getElementById('bulb');
  if (image.src.match('on')) {image.src = 'off';}
  else {image.src = 'on';}
}
</script>
</body>
</html>
)";
