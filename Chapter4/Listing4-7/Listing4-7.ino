/*******************************************************************************
 * Sketch name: GPS NMEA messages
 * Description: Display GPS message on Serial Monitor
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

int GPS_RX = 36, GPS_TX = 26;
int GPS_BAUD_RATE = 9600;
String str;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);
}
void loop()
{
  while (Serial1.available()) Serial.write(Serial1.read());
}
