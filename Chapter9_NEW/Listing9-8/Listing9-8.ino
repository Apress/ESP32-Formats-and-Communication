/*******************************************************************************
 * Sketch name: ESP-NOW transmitting ESP32
 * Description: Transmit smart meter reading to second ESP32
 * Created on:  September 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include <WiFi.h>            // include ESP-NOW and Wi-Fi 
#include <esp_wifi.h>         //   libraries
#include <esp_now.h>          // receiving ESP32 MAC address
uint8_t receiveMAC[] = {0xD4, 0xD4, 0xDA, 0x5A, 0xA4, 0x50};
//char ssid[] = "XXXX";         // replace with your router SSID
#include <ssid_password.h>
typedef struct            // structure for data
{
  int mV;             //   SCT013 voltage
  float battery;            //   battery voltage
  int channelPL;            //   router Wi-Fi channel
  int countPL;            //   data counter
  int rep;              //  repeated transmissions
} dataStruct;
dataStruct payload;
int battPin = 34, enabPin = 14, SCTpin = 37;      // battery pin when USB powered
int chk, scan, channel = 0;
int Nread = 500, volt, minVolt, maxVolt;    // number of current readings
RTC_DATA_ATTR int count = 0;        // store count in RTC memory
unsigned long micro = 60E6;       // shorthand 60106 for 60secs
unsigned long lag;

void setup()
{
  pinMode(enabPin, OUTPUT);
  digitalWrite(enabPin, HIGH);    // pin set HIGH to active ADC
  WiFi.mode(WIFI_STA);          // ESP32 in station mode
  scan = WiFi.scanNetworks();       // number of found Wi-Fi devices
  for (int i=0; i<scan; i++)
  {
    if(!strcmp(ssid, WiFi.SSID(i).c_str())) // compare to router SSID
    {
      channel = WiFi.channel(i);      // router Wi-Fi channel
      i = scan;           // exit the "for" loop
    }
  }
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_now_init();           // initialise ESP-NOW
  esp_now_peer_info_t receiver = {};    // establish ESP-NOW receiver
  memcpy(receiver.peer_addr, receiveMAC, 6);
  receiver.channel = channel;       // ESP-NOW receiver channel
  receiver.encrypt = false;
  esp_now_add_peer(&receiver);      // add ESP-NOW receiver
  esp_now_register_send_cb(sendData);   // sending data callback function
  esp_sleep_enable_timer_wakeup(micro);   // restart after fixed time
  taskFunction();           // call task function
  esp_now_send(receiveMAC, (uint8_t *) &payload, sizeof(payload));
  delay(500);         // interval for callback before deep sleep
  esp_deep_sleep_start();       // ESP32 in deep sleep mode
}

void taskFunction()         // manage data collection
{
  getCurrent();           // call function to measure current
  payload.battery = analogReadMilliVolts(battPin)*2.0/1000.0;
  payload.channelPL = channel;      // router Wi-Fi channel
  payload.countPL = count++;        // incremented counter
  payload.rep = 0;          // transmission repeats
}

void getCurrent()         // function to calculate current usage
{
  maxVolt = 0;            // minimum and maximum values
  minVolt = 5000;           // ignore initial readings
  for (int i=0; i<100; i++) analogReadMilliVolts(SCTpin);
  for (int i=0; i<Nread; i++)
  {
    volt = analogReadMilliVolts(SCTpin);    // SCT013 output voltage
    if(volt > maxVolt) maxVolt = volt;    // update maximum and
    if(volt < minVolt) minVolt = volt;    //    minimum voltages
  }
  payload.mV = maxVolt - minVolt;     // peak to peak mV
}

void sendData(const uint8_t * mac, esp_now_send_status_t chkS)
{               // function to count transmissions
  if(chkS != 0)           // transmission not received
  {
    payload.rep++;          // increment transmission number
    esp_now_send(receiveMAC, (uint8_t *) & payload, sizeof(payload));
  }               // re-transmit data
}

void loop()             // nothing in loop function
{}
