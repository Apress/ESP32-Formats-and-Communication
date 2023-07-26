const char page[] PROGMEM = R"+++(
<!doctype html><html><head>
<meta name='viewport' content='width=device-width,initial-scale=1.0'>
<meta charset='UTF-8'>
<title>ESP32</title>
<style>
body {font-family:Arial; text-align:center; margin-top:5px;}
table {margin: auto}
td {padding: 8px;}
.btn {display:block; width:100px; margin:auto; padding:10px;
      font-size:18px; color:white; text-decoration:none;}
.on {background-color: SteelBlue;}
.off {background-color: LightSteelBlue;}
</style>
</head>
<body id='initialise'>
<h1>ESP32 image streaming</h1>
<table align='center'><tr>
<td colspan = '2'><input type='radio' id='r1'
     onclick='changeLight()'>change light
<img id='bulb' width='25' height='50'
     src='https://www.w3schools.com/jsref/pic_bulboff.gif'>
<span id='lightId'>off</span></td>
</tr><tr>
<td><input autocomplete='on' type='range' min='20' max='160' value='20' 
     class='slider' id='Slider' oninput='sendSlider()'></td>
<td><label id='sliderId'>decrease - increase angle (20&deg)</label></td>
</tr><tr>
<td colspan = '3' align='center'><button class='btn off' id='F';
     onclick='direction("forward",id);'>forward</button></td>
</tr></table>
<table><tr>
<td align='center'><button class='btn off' id='L'
     onclick='direction("left",id);'>left</button></td>
<td align='center'><button class='btn on'  id='S'
     onclick='direction("stop",id);'>stop</button></td>
<td align='center'><button class='btn off' id='R'
     onclick='direction("right",id);'>right</button></td>
</tr><tr>
<td colspan = '3' align='center'><button class='btn off' id='B'
     onclick='direction("backward",id);'>backward</button></td>
</tr></table>
<script>
<img id='stream' src=''>
window.onload = document.getElementById('stream').src =
    'http://' + window.location.hostname + ':81/stream';
var sliderVal = 20;
var lightVal = 0;
var directVal = "null";
document.getElementById('initialise').onload = function() {init()};
function init()
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
}
function sendSlider()
{
  sliderVal = document.getElementById('Slider').value;
  document.getElementById('sliderId').innerHTML =
     'decrease - increase angle ('+ sliderVal.toString() + '&deg)';
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/update?device=servo&level='+ sliderVal, true);
  xhr.send();
}
function changeLight()
{
  lightVal = 1 - lightVal;
  if(lightVal == 1) {lightTag = 'on';}
  else {lightTag = 'off';}
  document.getElementById('lightId').innerHTML = lightTag;
  document.getElementById('r1').checked=false;
  var image = document.getElementById('bulb');
  if (image.src.match('bulboff'))
       {image.src = 'https://www.w3schools.com/js/pic_bulbon.gif';}
  else {image.src = 'https://www.w3schools.com/js/pic_bulboff.gif';}
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/update?device=light&level='+ lightVal, true);
  xhr.send();
}
function direction(direct,butn)
{
  document.getElementById('F').className = 'btn off'
  document.getElementById('B').className = 'btn off'
  document.getElementById('L').className = 'btn off'
  document.getElementById('R').className = 'btn off'
  document.getElementById('S').className = 'btn off'
  document.getElementById(butn).className = 'btn on'
  directVal = direct;
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/update?device=button&level='+ direct, true);
  xhr.send();
}
</script>
</body></html>
)+++";
