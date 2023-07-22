/*******************************************************************************
 * Sketch name: Bluetooth signal from ESP32 microcontroller
 * Description: Transmit audio on micro-SD card to Bluetooth speaker
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <AudioTools.h>          // include libraries to decode
#include <AudioLibs/AudioA2DP.h>      //   and stream MP3 files
#include <AudioLibs/AudioSourceSDFAT.h>
#include <AudioCodecs/CodecMP3Helix.h>
#include <Adafruit_SSD1306.h>       // Adafruit SSD1306 for OLED
int width = 128, height = 64;       // OLED dimensions
Adafruit_SSD1306 oled(width, height, &Wire, -1);    // associate oled with library
int Nfiles = 0, Nmp3 = 0;
int bootBtn = 0;            // ESP32 module BOOT button
const char *startFilePath = "/";      // top-level folder on SD card
const char* ext = "mp3";
AudioSourceSDFAT source(startFilePath, ext);  // source of audio data
A2DPStream out;    // streamed output
MP3DecoderHelix decoder;        // MP3 file decoder
AudioPlayer player(source, out, decoder);

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(bootBtn,INPUT_PULLUP);      // module BOOT button HIGH
//  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display I2C address
  oled.clearDisplay();
  oled.setTextColor(WHITE);       // set font colour
  oled.setTextSize(1);          // text size 68 pixels
  oled.display();
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);
  auto cfg = out.defaultConfig(TX_MODE);
  cfg.name = "Logitech BT Adapter";     // Bluetooth device name
  out.begin(cfg);
  player.setVolume(1.0);        // set volume between 0 and 1
  player.setMetadataCallback(trackData);    // call function to display data
  player.begin();
}

void trackData(MetaDataType type, const char* str, int len)
{
  String temp = String(toStr(type));
  Serial.printf("%s: %s \n", temp, str);    // display data on Serial Monitor
  if(temp == "Title")         // new audio track
  {
    oled.clearDisplay();
    oled.setCursor(0,0);
  }
  oled.printf("%s: %s \n", temp, str);    // display track data on OLED
  oled.display();
}

void loop()
{
  player.copy();            // handle audio data
  if(digitalRead(bootBtn) == LOW)     // when BOOT button pressed
  {
    Serial.println("boot Btn");
    player.stop();          // stop playing audio data
    delay(1000);
    player.next();          // play next audio MP3 file
  }
}
