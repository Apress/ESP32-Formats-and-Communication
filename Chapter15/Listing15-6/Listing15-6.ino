/*******************************************************************************
 * Sketch name: rtc library and current time
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <WiFi.h>
#include <ssid_password.h>
#define LILYGO_WATCH_2020_V2
#include <LilyGoWatch.h>        // TTGO T-Watch library
#include <soc/rtc.h>          // library for real-time clock
TTGOClass * ttgo;
struct tm timeData;
int GMT = 0, daylight = 3600;
unsigned long last = 0;
int hh, mm, ss;

void setup()
{
  Serial.begin(115200);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();            // initialise ttgo object
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  configTime(GMT, daylight, "uk.pool.ntp.org");
  getLocalTime(&timeData);        // get valid time data
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  if(millis() - last > 1000)
  {
    last = millis();
    RTC_Date timeData = ttgo->rtc->getDateTime(); // obtain current time
    hh = timeData.hour;
    mm = timeData.minute;         // access time variables
    ss = timeData.second;
    Serial.printf("%02d:%02d:%02d \n", hh, mm, ss);
  }
}
