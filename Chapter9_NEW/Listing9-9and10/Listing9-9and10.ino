/*******************************************************************************
 * Sketch name: ESP-NOW receiving ESP32
 * Description: Receiving ESP32 transmit to MQTT broker
 * Created on:  September 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include "properties.h"      // device and dashboard details
#include <TFT_eSPI.h>          // include TFT-eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
#include <WiFi.h>           // include Wi-Fi and ESP-NOW
#include <esp_now.h>          //    libraries
typedef struct            // structure for data
{
  int mV;             //   SCT013 voltage
  float battery;            //   battery voltage
  int channelPL;            //   router Wi-Fi channel
  int countPL;            //   data counter
  int rep;              //  repeated transmissions
} dataStruct;
dataStruct payload;
float mA, current, RMS, lag;
unsigned long last = 0;
#include <time.h>           // include time library
int GMT = 0, daylight = 3600;          // GMT and daylight saving offset
int hh, mm, ss, hhmm, hhmmOld;      //    in seconds
struct tm timeData;

void setup()
{
  WiFi.setSleep(WIFI_PS_NONE);      // prevent Wi-Fi sleep mode
  WiFi.mode(WIFI_AP_STA);       // access point and station mode
  WiFi.begin(ssid, password); // initialize and connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  esp_now_init();           // initialise ESP-NOW
  esp_now_register_recv_cb(receiveData);    // receiving data callback function
  initProperties();
  ArduinoCloud.begin(connHandle);   // connect to Arduino IoT Cloud
  setDebugMessageLevel(4);        // status of Arduino IoT Cloud connection
  ArduinoCloud.printDebugInfo();
  configTime(GMT, daylight, "uk.pool.ntp.org");   // NTP pool
  while (!getLocalTime(&timeData)) delay(500);  // wait for connection to NTP
  hhmmOld = 60*timeData.tm_hour + timeData.tm_min;  // set current hhmm value
  layout();             // function for LCD display labels
}
                // function to receive data
void receiveData(const uint8_t * mac, const uint8_t * data, int len)
{
  memcpy(&payload, data, sizeof(payload));  // copy data to payload structure
  mA = 0.5*payload.mV/22.0;       // convert SCT013 voltage
  current = mA*100.0/50.0;        //    to current
  RMS = current/sqrt(2.0);        // convert to RMS current
  power = 230.0 * RMS;          // convert to power (Watt)
  lag = (millis() - last)/1000.0;     // interval between receiving
  last = millis();          //    ESP-NOW transmissions
  getLocalTime(&timeData);        // update current time
  hh = timeData.tm_hour;
  mm = timeData.tm_min;
  ss = timeData.tm_sec;
  hhmm = 60*hh + mm;          // update hhmm value
  if(hhmm < hhmmOld) kWh = power/(3600.0*1000); // reset power for new day
  else kWh = kWh + power*lag/(3600.0*1000); // increment power
  hhmmOld = hhmm;
  varBattery = payload.battery;
  channel = payload.channelPL;
  countN = payload.countPL;
  display();          // function to display data on LCD screen
}

void layout()           // function for LCD display labels
{
  tft.init();           // initialise LCD screen
  tft.setRotation(3);         // landscape with USB on left
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);        // colours from TFT_eSPI.h
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.drawString("power", 0, 0, 4);     // labels for power and kWh
  tft.drawString("kWh", 0, 35, 4);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  tft.drawString("battery", 0, 70, 4);    // labels for battery and count
  tft.drawString("count", 0, 105, 4);
}

void display()          // function to display data on LCD screen
{
  tft.fillRect(100, 0, 140, 135, TFT_BLACK);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  String txt = String(power);       // convert variable to string
  tft.drawString(txt, 100, 0, 4);       // display current and
  txt = String(kWh);          //    cumulative power
  tft.drawString(txt, 100, 35, 4);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  txt = String(payload.battery);      // display battery voltage
  tft.drawString(txt, 100, 70, 4);
  txt = String(payload.countPL);      // display data counter
  tft.drawString(txt, 100, 105, 4);
  tft.setTextColor(TFT_RED,TFT_BLACK);
  tft.drawString("time", 190, 0, 4);    // display data reception time
  if(hh > 9) txt = String(hh); else txt = "0"+String(hh);
  tft.drawString(txt, 200, 35, 4);
  if(mm > 9) txt = String(mm); else txt = "0"+String(mm);
  tft.drawString(txt, 200, 70, 4);
  if(ss > 9) txt = String(ss); else txt = "0"+String(ss);
  tft.drawString(txt, 200, 105, 4);
}

void loop()
{
  ArduinoCloud.update();
}
