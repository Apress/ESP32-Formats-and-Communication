/*******************************************************************************
 * Sketch name: Restriction on GPS NMEA messages
 * Description: addition to Listing 4-7
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void loop()
{
  while (Serial1.available())
  {
    str = Serial1.readStringUntil('\n');
    if(str.startsWith("$GNRMC")) Serial.println(str);
  }
}
