/*******************************************************************************
 * Sketch name: Smart meter and MQTT
 * Description: Display smart meter readings on MQTT broker dashboard
 * Created on:  September 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include "properties.h"          // device and dashboard details
int battPin = 34, enabPin = 14, SCTpin = 37;
float RMS, mA, current;
int mV, volt, minVolt, maxVolt;
int Nread = 500;            // number of current readings
unsigned long lag, micro = 45E6;      // shorthand 45106 for 45secs
RTC_DATA_ATTR int count = 0;        // store count in RTC memory
int flag = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(enabPin, OUTPUT);
  digitalWrite(enabPin, HIGH);      // pin set HIGH to active ADC
  lag = millis();
  initProperties();         // Arduino IoT Cloud properties
  WiFi.mode(WIFI_AP_STA);       // access point and station mode
  WiFi.begin(ssid, password);       // initialize and connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  ArduinoCloud.begin(connHandle);       // connect to Arduino IoT Cloud
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onConnect);
  setDebugMessageLevel(4);        // status of Arduino IoT Cloud
  ArduinoCloud.printDebugInfo();      //   connection
  esp_sleep_enable_timer_wakeup(micro);
}               // delay of 45s  restart at 60s

void onConnect()
{
  cnectTime = millis() - lag;       // connect time to MQTT broker
  Serial.print("connect status ");Serial.println(ArduinoCloud.connected());
}

void taskFunction()
{
  battery = analogReadMilliVolts(battPin)*2.0/1000.0; // battery voltage
  count++;
  countN = count;
  getCurrent();           // call function to measure current
}

void getCurrent()         // function to calculate current usage
{
  maxVolt = 0;
  minVolt = 5000;
  for (int i=0; i<100; i++) analogRead(SCTpin); // ignore initial readings
  for (int i=0; i<Nread; i++)
  {
    volt = analogReadMilliVolts(SCTpin);    // SCT013 output voltage
    if(volt > maxVolt) maxVolt = volt;    // update maximum and
    if(volt < minVolt) minVolt = volt;    //   minimum voltages
  }
  mV = maxVolt - minVolt;       // peak to peak mV
  mA = 0.5*mV/22.0;         // mV with burden resistor to mA
  current = mA*100.0/50.0;        // mA to current usage in amps
  RMS = current/sqrt(2.0);        // RMS current usage
  power = 230.0 * RMS;          // convert current to power
}

void loop()
{
  ArduinoCloud.update();        // Arduino IoT Cloud update
  if(ArduinoCloud.connected() == 1 && flag == 0)
  {
    taskFunction();       // call taskFunction once connected
    flag = 1;         // flag to only set lag value once
    lag = millis();
  }             // time to pass data to Arduino IoT Cloud
  if((millis() - lag) > 5000 && flag == 1)
  {
    WiFi.disconnect();        // disconnect Wi-Fi
    WiFi.mode(WIFI_OFF);
    esp_deep_sleep_start();     // ESP32 in deep sleep mode
  }
}
