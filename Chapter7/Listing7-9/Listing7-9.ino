/*******************************************************************************
 * Sketch name: Addition of readLED function
 * Description: Extension to Listing 7-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

void readLED()        // function to broadcast LED states to clients
{
  strG = String(digitalRead(LEDGpin));      // determine LED states
  strR = String(digitalRead(LEDRpin));
  strG = (strG == "1" ? "ON" : "OFF");      // convert LED states
  strR = (strR == "1" ? "ON" : "OFF");      //     to "ON" or "OFF"
  json =  "{\"var1\": \"" + strG + "\",";     // partition with comma
  json += " \"var2\": \"" + strR + "\"}";     // end with close bracket
  websocket.broadcastTXT(json.c_str(), json.length());   // broadcast to clients
}
