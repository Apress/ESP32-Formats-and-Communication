/*******************************************************************************
 * Sketch name: LoRa transmit
 * Description: Transmit to receiving LoRa device
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    6 - LoRa and microsatellites
 ******************************************************************************/

#include "logo.h"            // tab with LoRa logo
#include <LoRa.h>           // include LoRa library and
#include <Adafruit_SSD1306.h>       // Adafruit SSD1306 for OLED
int width = 128;            // OLED dimensions
int height = 64;
Adafruit_SSD1306 oled(width, height, &Wire, -1);    // associate oled with library
int LEDpin = 25;            // built-in LED
int battPin = 35;           // ADC for battery voltage
float battery;
unsigned long lastTime;
String txt, packet, recv;
int counter = 0, packetSize;

void setup()
{
  pinMode(LEDpin, OUTPUT);        // define input and output GPIO
  pinMode(battPin, INPUT);
  digitalPinToInterrupt(LORA_IRQ);      // set pin as interrupt
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);  // define LoRa module pins
  LoRa.setSpreadingFactor(9);       // define spreading factor
  LoRa.setSignalBandwidth(62.5E3);      // set bandwidth to 62.5kHz
  while (!LoRa.begin(433E6)) delay(500);    // 433MHz transmission
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display I2C address
  oled.setTextColor(WHITE);       // set font colour
  oled.setTextSize(1);          // text size 6x8 pixels
  oled.display();
}
void loop()
{
  if(millis() - lastTime > 5000)      // 5s transmission interval
  {
    lastTime = millis();        // update transmission time
    digitalWrite(LEDpin, HIGH);
    battVolt();           // call battery voltage function
    screen();           //     and OLED display function
    if(counter % 2 == 0) txt = "ABC";
    else txt = "FGH";
    packet = "#A"+txt+String(counter)+"Z#"; // create packet
    counter++;
    digitalWrite(LEDpin, LOW);
    while(counter > 8) {};
    LoRa.beginPacket();         // start LoRa transmission
    LoRa.print(packet);         // transmit packet
    LoRa.endPacket();         // close LoRa transmission
  }
//  digitalWrite(LEDpin, LOW);
  packetSize = LoRa.parsePacket();      // detect received packet
  if (packetSize > 0)
  {
    recv = "";            // read packet
    while(LoRa.available()) recv = LoRa.readString();
    screen();
  }
}

void screen()           // function for OLED display
{
  oled.clearDisplay();          // display LoRa logo
  oled.drawBitmap(30, 0, logo, logoWidth, logoHeight, WHITE);
  oled.setCursor(0,35);
  oled.printf("sent %s \n", packet);   // display transmitted value
  oled.setCursor(0,45);
  oled.print(recv);         // received confirmation message
  oled.setCursor(0,55);
  oled.printf("battery %4.0fmV \n", battery);  // display battery voltage
  oled.display();
}

void battVolt()          // function to measure battery voltage
{
  battery = 0;            // average of 20 readings
  for (int i=0; i<20; i++)
    battery = battery + analogReadMilliVolts(battPin);
  battery = battery*2.0/20.0;
}
