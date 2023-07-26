/*******************************************************************************
 * Sketch name: Change image resolution function
 * Description: Extension to Listing 11-15
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

function changeFrame()
{
  const frames = ["96x96",
  "QQVGA 160x120","QCIF 176x144","HQVGA 240x176","240x240",
  "QVGA 320x240", "CIF 400x296", "HVGA 480x320", "VGA 640x480",
  "SVGA 800x600", "XGA 1024x768","HD 1280x720",  "SXGA 1280x1024",
  "UXGA 1600x1200"];
  frameVal = document.getElementById('frame').value;
  document.getElementById('frameId').innerHTML =
     'framesize ' + frames[frameVal]; // frameVal.toString();
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/update?device=frame&level='+ frameVal, true);
  xhr.send();
}
