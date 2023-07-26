/*******************************************************************************
 * Sketch name: LoRa receive
 * Description: Receive from transmitting device
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    6 - LoRa and microsatellites
 ******************************************************************************/

#include "logo.h"            // tab with LoRa logo
#include <LoRa.h>           // include LoRa library
#include <SD.h>           //     and SD library
File file;              // associate file with SD library
SPIClass hspi(HSPI);          // SD card uses HSPI
#include <Adafruit_SSD1306.h>       // Adafruit SSD1306 for OLED
int width = 128;            // OLED dimensions
int height = 64;
Adafruit_SSD1306 oled(width, height, &Wire, -1);    // associate oled with library
int LEDpin = 25;            // module LED
int battPin = 35;           // ADC GPIO for battery
int count = 0, SDcard = 0, SDsize;
uint8_t SDtype;
String packet, filename = "/LoRadata.txt";  // filename for data
String text, header = "lag,RSSI,SNR,msg,bytes,battery"; // file header
int RSSI, dist, packetSize, interval;
float SNR, battery;
unsigned long lastTime = 0, chkTime = 0;

void setup()
{
  pinMode(LEDpin, OUTPUT);        // define input and output GPIO
  pinMode(battPin, INPUT);
  digitalPinToInterrupt(LORA_IRQ);      // set pin as interrupt
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);  // define LoRa module pins
  while (!LoRa.begin(433E6)) delay(500);    // 433MHz transmission
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display I2C address
  oled.setTextColor(WHITE);       // set font colour
  oled.setTextSize(1);          // text size 6x8 pixels
  oled.clearDisplay();
  oled.drawBitmap(30, 0, logo, logoWidth, logoHeight, WHITE);
  oled.setCursor(0, 35);
  hspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  delay(50);              // time to initialise HSPI
  if(!SD.begin(SD_CS, hspi))        // check for presence of SD card
  {
    oled.print("SD card error");      // do nothing, if no SD card
    oled.display();
  }
  else              // when SD card present
  {
    SDcard = 1;
    SDsize = SD.cardSize()/(1024*1024);   // SD card size in MB
    SDtype = SD.cardType();       // get SD card type
    if(SDtype == CARD_MMC) oled.print("SD type MMC");
    else if(SDtype == CARD_SD) oled.print("SD type SDSC");
    else if(SDtype == CARD_SDHC) oled.print("SD type SDHC");
    else oled.print("SD type UNKNOWN");
    oled.setCursor(0, 45);
    oled.printf("SD size %d MB \n", SDsize);  // display card type and size
    oled.display();
    if(SD.exists(filename)) SD.remove(filename);  // delete existing file
    file = SD.open(filename, FILE_WRITE);     // create new file
    file.print(header);           // write header to file
    file.println();
    file.close();
  }
  delay(2000);        // time for user to read SD card information
}

void loop()
{
  packetSize = LoRa.parsePacket();      // detect received packet
  if(packetSize > 0)
  {
    packet = "";            // read packet and build message
    while(LoRa.available()) packet = packet + (char)LoRa.read();
    if (packet.startsWith("#A") && packet.endsWith("Z#")) 
    {               // packet = #A value Z# not noise
      digitalWrite(LEDpin, HIGH);
      battVolt();           // call battery voltage function
      interval = round((millis() - lastTime)/1000);  // interval between packets
      lastTime = millis();        // update transmission time
      RSSI = LoRa.packetRssi();     // signal RSSI
      SNR = LoRa.packetSnr();       // signal to noise ratio
      dist = round(pow(10, (-69.0-RSSI)/20.0));
      LoRa.beginPacket();       // start LoRa transmission
      LoRa.print("recv " + packet);     // transmit packet
      LoRa.endPacket();         // close LoRa transmission
      if(SDcard > 0)
      {             // write data to SD card
        text = String(interval)+","+String(RSSI)+","+String(SNR)+",";
        text = text + String(packet)+","+String(packetSize)+",";
        text = text + String(battery, 0);
        file = SD.open(filename, FILE_APPEND);
        file.print(text);
        file.println();
        file.close();
      }
      screen();           // OLED display function
      digitalWrite(LEDpin, LOW);      // no need for delay
    }
  }
}

void screen()           // function for OLED display
{
    oled.clearDisplay();        // display LoRa logo
    oled.drawBitmap(75, 0, logo, logoWidth, logoHeight, WHITE);
    oled.setCursor(0,0);
    oled.printf("lag   %d \n", interval);
    oled.setCursor(0,10);
    oled.printf("RSSI  %d \n", RSSI);   // display interval, RSSI and SNR
    oled.setCursor(0,20);
    oled.printf("SNR   %4.2f \n", SNR);
    oled.setCursor(0,30);
    oled.printf("dist  %d m \n", dist);
//  oled.printf("bytes %d \n", packetSize); // display received bytes
    oled.setCursor(0,40);
    oled.print("msg   ");       // display edited message
    oled.print(packet.substring(2, packet.length()-2));
    oled.setCursor(0,50);
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
