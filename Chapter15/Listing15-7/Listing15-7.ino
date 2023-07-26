/*******************************************************************************
 * Sketch name: TimeLib and NTPtimeESP libraries and current time
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <WiFi.h>
#include <ssid_password.h>
#include <TimeLib.h>          // include TimeLib and
#include <NTPtimeESP.h>         //     NTPtimeESP libraries
NTPtime NTP("uk.pool.ntp.org");     // define NTP
strDateTime dateTime;         // NTPtimeESP library structure
int GMT = 0, daylight = 1;        // GMT, daylight saving (hr)
unsigned long last;
int wd, hh, mm, ss, dd, mn, yy;
String days[] = {"Sun","Mon","Tues","Wed","Thur","Fri","Sat"};
String mon[] = {"Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"};

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  dateTime = NTP.getNTPtime(GMT, daylight);   // obtain valid NTP time
  while (!dateTime.valid) dateTime = NTP.getNTPtime(0, 1);
  setTime(dateTime.epochTime);      // TimeLib library command
  WiFi.disconnect(true);        // disconnect Wi-Fi
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  if(millis() - last > 1000)
  {
    last = millis();
    wd = weekday();         // day of week starts at 1
    hh = hour();
    mm = minute();
    ss = second();
    dd = day();
    mn = month();           // month starts at 1
    yy = year();            // actual year value
    Serial.printf("%s %02d:%02d:%02d %02d %s %d \n",
     days[wd-1], hh, mm, ss, dd, mon[mn-1], yy);
  }
}
