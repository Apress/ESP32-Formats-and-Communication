/*******************************************************************************
 * Sketch name: Time library and current time (1)
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <WiFi.h>            // include Wi-Fi library
#include <ssid_password.h>        // file with logon details
#include <time.h>           // include time library
struct tm timeData;         // structure with time data
int GMT = 0, daylight = 3600;       // GMT, daylight saving (sec)
unsigned long last;
int wd, hh, mm, ss, dd, mn, yy;
String days[] = {"Sun","Mon","Tues","Wed","Thur","Fri","Sat"};
String mon[] = {"Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"};

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);     // initialise and connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  configTime(GMT, daylight, "uk.pool.ntp.org");   // NTP pool
  while (!getLocalTime(&timeData)) delay(500);    // get valid time
  WiFi.disconnect(true);        // disconnect Wi-Fi
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  if(millis() - last > 1000)        // at every second
  {
    last = millis();
    getLocalTime(&timeData);        // obtain current time
    wd = timeData.tm_wday;        // day of week starts at 0
    hh = timeData.tm_hour;
    mm = timeData.tm_min;
    ss = timeData.tm_sec;
    dd = timeData.tm_mday;        // day in month
    mn = timeData.tm_mon;       // month starts at 0
    yy = timeData.tm_year + 1900;
    Serial.printf("%s %02d:%02d:%02d %02d %s %d \n",
 days[wd], hh, mm, ss, dd, mon[mn], yy);
  }
}
