char page[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<body>
<img id='bulb' onclick='switchImage()'
  src='https://www.w3schools.com/js/pic_bulboff.gif' width='100' height='180'>
<p>Click the bulb to turn on or off</p>
<script>
function switchImage()
{
  var image = document.getElementById('bulb');
  if (image.src.match('bon'))
       {image.src = 'https://www.w3schools.com/js/pic_bulboff.gif';}
  else {image.src = 'https://www.w3schools.com/js/pic_bulbon.gif';}
}
</script>
</body>
</html>
)";
