/*******************************************************************************
 * Sketch name: Time library and current time (2)
 * Description: Based on epoch time
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <WiFi.h>
#include <ssid_password.h>
#include <time.h>
time_t epoch;              // time library variable
struct tm timeData;
int GMT = 0, daylight = 3600;
unsigned long last;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  configTime(GMT, daylight, "uk.pool.ntp.org");
  while (time(nullptr)< 1000) delay(500);   // wait for connection to NTP
  epoch = time(nullptr);        // set the Unix epoch time
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  if(millis() - last > 1000)
  {
    last = millis();
    time(&epoch);           // current Unix epoch time
    Serial.print(ctime(&epoch));      // time in default format
    localtime_r(&epoch, &timeData);   // convert Unix epoch time to structure
    Serial.print("time ");Serial.println(&timeData, "%H:%M:%S");
    Serial.printf("sec %d \n\n", timeData.tm_sec);
  }
}
