/*******************************************************************************
 * Sketch name: HTML and AJAX code for webpage
 * Description: HTML code for Listing 11-7
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

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
window.onload = document.getElementById('stream').src =
    window.location.href.slice(0, -1) + ':81/stream';
</script>
</body>
</html>
)+++";
