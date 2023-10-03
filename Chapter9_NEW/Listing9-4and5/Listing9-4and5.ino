/*******************************************************************************
 * Sketch name: TVOC and CO2 measurement and Arduino IoT Cloud MQTT broker
 * Description: Extend Listing 9-1 to include MQTT broker
 * Created on:  September 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/
#include "properties.h"      // details of ESP32 device and dashboard variables
#include <Wire.h>            // include Wire and
#include <ccs811.h>;          // CCS811 libraries
CCS811 ccs811(19);          // CCS811 nWAKE on GPIO 19
uint16_t CO2, TVOC, errstat, rawdata;
int LEDMQTTpin = 4;         // LED controlled with MQTT
int flag = 0;            // flag to indicate high CO2

void setup()
{
  Serial.begin(115200);
  initProperties();              // Arduino IoT Cloud properties
  ArduinoCloud.begin(connHandl);   // connect to Arduino IoT Cloud
  pinMode(LEDMQTTpin, OUTPUT);      // define LED pin as output
  digitalWrite(LEDMQTTpin, LOW);      // turn off LED
  Wire.begin();           // initialise I2C
  ccs811.begin();           // initialise CCS811
  ccs811.start(CCS811_MODE_10SEC);      // set reading interval at 10s
  setDebugMessageLevel(4);        // status of Arduino IoT Cloud 
  ArduinoCloud.printDebugInfo();      //   connection
}

void loop()
{
  ArduinoCloud.update();         // Arduino IoT Cloud update
  ccs811.read(&CO2, &TVOC, &errstat, &rawdata); // read CCS811 sensor data
  if(errstat == CCS811_ERRSTAT_OK)      // given valid readings
  {
    varCO2 = CO2;
    varTVOC = TVOC;
    Serial.printf("CO2 %d TVOC %d \n", varCO2, varTVOC);
    CO2change();               // call CO2change function
  }
}

void CO2change()            // function to set LED state
{
  if(CO2 > 1000 && flag == 0)     
  {
    flag = 1;
    Serial.println("trigger = 1");
    LED = 1;  // only changes switch state on dashboard
    LEDchange();  // required to change LED state
  }
  else if(CO2 < 1000 && flag == 1)
  {
    flag = 0;  // reset flag when variable < threshold
    Serial.println("trigger = 0");
  }
}

void LEDchange()            // function to turn on or off LED
{               // LED state set by MQTT broker
  digitalWrite(LEDMQTTpin, LED);
  if(LED == 1) Serial.println("LED ON");
  else Serial.println("LED OFF");
}
