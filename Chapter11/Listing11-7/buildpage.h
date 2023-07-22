const char page[] PROGMEM = R"+++(
<!doctype html><html><head>
<meta name='viewport' content='width=device-width,initial-scale=1.0'>
<meta charset='UTF-8'>
<title>ESP32</title>
<style>
body {font-family:Arial; text-align:center; margin-top:5px;}
</style>
</head>
<body>
<h1>ESP32 image streaming</h1>
<img id='stream' src=''>
<script>
var site = 'http://' + window.location.hostname + ':81/stream';
window.onload = document.getElementById('stream').src = site;
</script>
</body>
</html>
)+++";
