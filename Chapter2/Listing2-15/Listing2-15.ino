/*******************************************************************************
 * Sketch name: Bluetooth signal from M5Stack Core2
 * Description: Transmit audio on micro-SD card to Bluetooth speaker
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>          // include M5Core2 library
#include <AudioTools.h>
#include <AudioLibs/AudioA2DP.h>
#include <AudioLibs/AudioSourceSDFAT.h>
#include <AudioCodecs/CodecMP3Helix.h>
const char *startFilePath = "/";
const char* ext = "mp3";            // M5Stack Core2
int SD_CS = 4, SD_SCK = 18, SD_MISO = 38, SD_MOSI = 23; //   SPI pins

// github.com/pschatzmann/arduino-audio-tools/issues/571
SdSpiConfig sdcfg(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10));  // define SD card
AudioSourceSDFAT source(startFilePath, ext, sdcfg);   //   as SPI source
A2DPStream out;
MP3DecoderHelix decoder;
AudioPlayer player(source, out, decoder);
float vol = 0.5;            // initial volume setting

void setup()
{
  M5.begin();           // initialise M5Stack module
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);   // initialise SPI
  SD.begin(SD_CS);            //   and SD card reader
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);
  auto cfg = out.defaultConfig(TX_MODE);
  cfg.name = "Logitech BT Adapter";
  out.begin(cfg);
  player.setVolume(vol);
  player.setMetadataCallback(trackData);
  player.begin();
}

void trackData(MetaDataType type, const char* str, int len)
{
  Serial.printf("%s: %s \n", toStr(type), str);
}

void loop()
{
  player.copy();
  if(M5.BtnA.wasPressed())        // increase volume after
  {               //   button A was pressed
    vol = vol + 0.1;
    if(vol > 1.0) vol = 0;
    player.setVolume(vol);
    Serial.printf("Volume %3.1f \n", vol);
  }
  if(M5.BtnB.wasPressed())        // after button B pressed
  {               //   move to next track
    player.stop();
    delay(1000);
    player.next();
  }
  M5.update();            // update button "pressed" states
}
