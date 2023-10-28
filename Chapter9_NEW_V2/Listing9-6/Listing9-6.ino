/*******************************************************************************
 * Sketch name: TVOC and CO2 measurement and Adafruit IO MQTT broker
 * Description: Three methods for notification of LED state change
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
AdafruitIO_Feed *redLEDin = adaIO.feed("redledout");  // data feed key defined
AdafruitIO_Feed *blueLEDin = adaIO.feed("blueledout"); //   for ->get() command
AdafruitIO_Feed *LDRout = adaIO.feed("LDRin");
AdafruitIO_Feed *emailout = adaIO.feed("emailin");

int redLEDpin = 26;         // LED and LDR GPIO pins
int blueLEDpin = 27;
int LDR, LDRpin = 35;
unsigned long lag = 0;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(redLEDpin, OUTPUT);       // define pins as OUTPUT
  pinMode(blueLEDpin, OUTPUT);      //   or INPUT
  pinMode(LDRpin, INPUT);
  adaIO.connect();          // connect to io.adafruit.com
  redLEDin->onMessage(redLED_fn);     // call function when message
  blueLEDin->onMessage(blueLED_fn);     // received from Adafruit IO
  while(adaIO.status() < AIO_CONNECTED)   // wait for connection
  {               //   to Adafruit IO
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(adaIO.statusText());   // "Adafruit IO connected"
  redLEDin->get();          // get dashboard states
  blueLEDin->get();         // of red and blue LEDs
}

void redLED_fn(AdafruitIO_Data *data)   // function called when LED
{               //   state changed on dashboard
  Serial.print("red LED state ");     
  if(data->toPinLevel() == HIGH) Serial.println("HIGH");
  else Serial.println("LOW");
  digitalWrite(redLEDpin, data->toPinLevel());  // turn on or off LED
}

void blueLED_fn(AdafruitIO_Data *data)
{
  Serial.print("blue LED state ");
  if(data->toPinLevel() == HIGH) Serial.println("HIGH");
  else Serial.println("LOW");
  digitalWrite(blueLEDpin, data->toPinLevel());
  int LED = data->toInt();        // return data feed to Adafruit IO
  emailout->save(LED);          //   to trigger IFTTT email
}

void loop()
{
  adaIO.run();        // maintain client connection to io.adafruit.com
  if(millis() - lag > 10000)        // after 10s intervals
  {
    LDR = analogRead(LDRpin);       // update LDR reading
    Serial.printf("LDRout %d \n", LDR);
    LDRout->save(LDR);          // send data feed to Adafruit IO
    lag = millis();
  }
}
