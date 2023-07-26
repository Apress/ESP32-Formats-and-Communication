/*******************************************************************************
 * Sketch name: Smart meter and MQTT
 * Description: Display smart meter readings on MQTT broker dashboard
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include <WiFi.h>
#include <CayenneMQTTESP32.h>        // Cayenne MQTT library
#include <ssid_password.h>        // file with logon details
int battPin = 34, enabPin = 14, SCTpin = 37;
float mA, current, RMS, battery;
int volt, minVolt, maxVolt, mV, power;
RTC_DATA_ATTR int count = 0;        // store count in RTC memory
int Nread = 1000;           // number of current readings
unsigned long cnectTime = 0, micro = 60E6;  // shorthand 60106 for 60secs

void setup()
{
  pinMode(enabPin, OUTPUT);
  digitalWrite(enabPin, HIGH);
  esp_sleep_enable_timer_wakeup(micro);   // restart after 60s
  cnectTime = millis();
  Cayenne.begin(username, mqttpass, clientID, ssid, password);
  cnectTime = millis() - cnectTime;     // 3.5 - 4s to re-connect
  taskFunction();           // call task function
  delay(3000);            // time to complete sending data 
  esp_deep_sleep_start();       // ESP32 in deep sleep mode
}

void taskFunction()
{
  battery = analogReadMilliVolts(battPin)*2.0/1000.0; // battery voltage
  count++;
  getCurrent();           // call function to measure current
  Cayenne.virtualWrite(V3, RMS, "prox", "");  // send data to MQTT
  Cayenne.virtualWrite(V4, mV, "prox", "");
  Cayenne.virtualWrite(V5, power, "prox", "");
  Cayenne.virtualWrite(V6, battery, "prox", "");
  Cayenne.virtualWrite(V7, count, "prox", "");
  Cayenne.virtualWrite(V8, cnectTime, "prox", "");
}

void getCurrent()         // function to calculate current usage
{
  maxVolt = 0;
  minVolt = 5000;
  for (int i=0; i<200; i++) analogRead(SCTpin);   // ignore initial readings
  for (int i=0; i<Nread; i++)
  {
    volt = analogReadMilliVolts(SCTpin);    // SCT013 output voltage
    if(volt > maxVolt) maxVolt = volt;    // update maximum and
    if(volt < minVolt) minVolt = volt;    //     minimum voltages
  }
  mV = maxVolt - minVolt;       // peak to peak mV
  mA = 0.5*mV/22.0;         // mV with burden resistor to mA
  current = mA*100.0/50.0;        // mA to current usage in amps
  RMS = current/sqrt(2.0);        // RMS current usage
  power = 230.0 * RMS;          // convert current to power
}

void loop()             // nothing in loop function
{}
