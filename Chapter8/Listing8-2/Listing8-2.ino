/*******************************************************************************
 * Sketch name: Parsing a JSON formatted string
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

   DeserializationError error = deserializeJson(jsonDoc, RXmsg);
// DeserializationError error = deserializeJson(jsonDoc, RXmsg.data());
   height = jsonDoc["val1"];
   width  = jsonDoc["val2"];
   length = jsonDoc["val3"];
   weight = jsonDoc["val4"];
