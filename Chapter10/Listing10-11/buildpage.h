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
