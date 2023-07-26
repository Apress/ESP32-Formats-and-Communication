/*******************************************************************************
 * Sketch name: TVOC and CO2 measurement and MQTT broker
 * Description: Extend Listing 9-1 to include MQTT broker
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include <Wire.h>            // include Wire and
#include <ccs811.h>;          // CCS811 libraries
#include <CayenneMQTTESP32.h>       // Cayenne MQTT library
#include <ssid_password.h>        // file with logon details
//char username[] = "xxxx";     // change xxxx to Cayenne username
//char mqttpass[] = "xxxx";     // change xxxx to Cayenne password
//char clientID[] = "xxxx";     // change xxxx to Cayenne client identity
CCS811 ccs811(19);          // CCS811 nWAKE on GPIO 19
uint16_t CO2, TVOC, errstat, rawdata;
int LEDpin = 2;           // LED as activity indicator
int LEDMQTTpin = 4;         // LED controlled with MQTT
int count = 0, countDown = 200;
unsigned long LEDtime = 0, countTime;

void setup()
{               // initiate Cayenne MQTT
  Cayenne.begin(username, mqttpass, clientID, ssid, password);
  pinMode(LEDpin, OUTPUT);        // define LED pins as output
  digitalWrite(LEDpin, LOW);        // turn off LEDs
  pinMode(LEDMQTTpin, OUTPUT);
  digitalWrite(LEDMQTTpin, LOW);
  Wire.begin();           // initialise I2C
  ccs811.begin();           // initialise CCS811
  ccs811.start(CCS811_MODE_60SEC);      // set reading interval at 60s
}

void loop()
{
  Cayenne.loop();           // Cayenne loop function
  ccs811.read(&CO2, &TVOC, &errstat, &rawdata); // read CCS811 sensor data
  if(errstat == CCS811_ERRSTAT_OK)      // given valid readings
  {
    count++;            //increment reading counter
    countDown = 60;
    countTime = millis();       // set countdown time
    Cayenne.virtualWrite(V3, CO2, "prox", "");  // send data to MQTT
    Cayenne.virtualWrite(V4, TVOC, "prox", "");
    Cayenne.virtualWrite(V6, count, "prox","");
    Cayenne.virtualWrite(V7, countDown, "prox","");
    LEDtime = millis();
    digitalWrite(LEDpin, HIGH);     // turn on indicator LED
  }               // turn off LED after 100ms
  if(millis() - LEDtime > 100) digitalWrite(LEDpin, LOW);
  if(millis() - countTime > 5000)     // countdown interval 5s
  {
    countTime = millis();       // update countdown time
    countDown = countDown - 5;      // update countdown
    Cayenne.virtualWrite(V7, countDown, "prox",""); // send to MQTT
  }
}

CAYENNE_IN(0)           // Cayenne virtual channel 0
{
  digitalWrite(LEDMQTTpin, getValue.asInt()); // turn on or off LED
}
