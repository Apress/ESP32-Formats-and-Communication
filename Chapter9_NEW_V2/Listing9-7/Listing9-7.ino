/*******************************************************************************
 * Sketch name: Map function of Adafruit IO MQTT broker
 * Description: Display map on Adafruit IO dashboard
 * Created on:  November 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/
#include "AdafruitIO_WiFi.h"        // include library
#define IO_USERNAME "xxxx"        // change xxxx to your
#define IO_KEY      "xxxx"        // IO_USERNAME and IO_KEY
#include <ssid_password.h>
AdafruitIO_WiFi adaIO(IO_USERNAME, IO_KEY, ssid, password);
AdafruitIO_Feed *location = adaIO.feed("location");

unsigned long lag = 0;
int count = 0;
double lat = 55.953251;         // initial latitude
double lon = -3.188267;         //   and longitude
double elev = 0;            // m above sea level

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  adaIO.connect();          // connect to io.adafruit.com
  while(adaIO.status() < AIO_CONNECTED)   // wait for connection
  {               //   to Adafruit IO
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(adaIO.statusText());   // "Adafruit IO connected"
}

void loop()
{
  adaIO.run();        // maintain client connection to io.adafruit.com
  if(millis() - lag > 10000)        // after 10s intervals
  {
    count++;            // increment counter,
    lat = lat + 0.001;          //   latitude
    lon = lon + 0.001;          //   and longitude
    location->save(count, lat, lon, elev);  // data feed to Adafruit IO
    lag = millis();
  }
}
