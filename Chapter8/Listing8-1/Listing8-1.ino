/*******************************************************************************
 * Sketch name: JSON formatting data
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

String JsonConvert(int val1, int val2, int val3, int val4)
{
  json  = "{\"val1\": \"" + String(val1) + "\",";
  json += " \"val2\": \"" + String(val2) + "\",";
  json += " \"val3\": \"" + String(val3) + "\",";
  json += " \"val4\": \"" + String(val4) + "\"}";
  return json;
}
