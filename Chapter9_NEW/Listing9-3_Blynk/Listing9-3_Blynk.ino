/*******************************************************************************
 * Sketch name: Smart meter and MQTT
 * Description: Display smart meter readings on MQTT broker dashboard
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/
#define BLYNK_TEMPLATE_ID   "TMPL5C9TS0yze"
#define BLYNK_TEMPLATE_NAME "Listing 93 template"
#define BLYNK_AUTH_TOKEN    "XLW9wTwpdOEY8pgC6bt03olLtyatEqHz"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>      // Blynk MQTT library
#include <ssid_password.h>        // file with logon details
int battPin = 34, enabPin = 14, SCTpin = 37;
float mA, current, RMS, battery;
int volt, minVolt, maxVolt, mV, power;
RTC_DATA_ATTR int count = 0;        // store count in RTC memory
int Nread = 1000;           // number of current readings
unsigned long cnectTime = 0, micro = 60E6;  // shorthand 60106 for 60secs

BLYNK_CONNECTED()  // function called when ESP32 connected
{
  Serial.println("ESP32 TTGO connected");
}

void setup()
{
  Serial.begin(115200);
  pinMode(enabPin, OUTPUT);
  digitalWrite(enabPin, HIGH);
  esp_sleep_enable_timer_wakeup(micro);   // restart after 60s
  cnectTime = millis();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssidEXT, password, "blynk.cloud", 80);
  cnectTime = millis() - cnectTime;     // 3.5 - 4s to re-connect
  taskFunction();           // call task function
  Serial.printf("count %d cnect ", count);Serial.println(cnectTime);
  delay(3000);            // time to complete sending data 
  esp_deep_sleep_start();       // ESP32 in deep sleep mode
}

void taskFunction()
{
  battery = analogReadMilliVolts(battPin)*2.0/1000.0; // battery voltage
  count++;
  getCurrent();           // call function to measure current
  Blynk.virtualWrite(V3, RMS);  // send data to MQTT
  Blynk.virtualWrite(V4, mV);
  Blynk.virtualWrite(V5, power);
  Blynk.virtualWrite(V6, battery);
  Blynk.virtualWrite(V7, count);
  Blynk.virtualWrite(V8, cnectTime);
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
{
  Blynk.run();    // maintains connection to Blynk
}
