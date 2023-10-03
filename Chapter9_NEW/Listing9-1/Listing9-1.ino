/*******************************************************************************
 * Sketch name: TVOC and CO2 measurement
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/

#include <ccs811.h>          // include CCS811 and
#include <Wire.h>           // Wire libraries
CCS811 ccs811(19);          // nWAKE connected to GPIO 19
uint16_t CO2, TVOC, errstat, rawdata;
unsigned long last, diff;
int LEDpin = 2, LED = 0;        // LED as activity indicator

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LED);        // turn off LED
  Wire.begin();           // initialise I2C
  ccs811.begin();           // initialise CCS811
  ccs811.start(CCS811_MODE_10SEC);      // set reading interval at 10s
}

void loop()
{
  ccs811.read(&CO2, &TVOC, &errstat, &rawdata); // read CCS811 sensor data
  if(errstat == CCS811_ERRSTAT_OK)      // given valid readings
  {
    diff = millis() - last;
    last = millis();          // interval since last reading
    LED = 1-LED;            // turn on or off indicator LED
    digitalWrite(LEDpin, LED);      // display readings
    Serial.printf("\n int %d CO2 %dppm TVOC %dppb \n", diff, CO2, TVOC);
  }               // print dot between readings
  else if(errstat == CCS811_ERRSTAT_OK_NODATA) Serial.print(".");
  else if(errstat & CCS811_ERRSTAT_I2CFAIL) Serial.println("I2C error");
  else
  {               // display error message
    Serial.print("errstat = "); Serial.print(errstat,HEX);
    Serial.print(" = "); Serial.println(ccs811.errstat_str(errstat));
  }
  delay(1000);            //arbitrary delay of 1s to display
}               //    dot between readings
