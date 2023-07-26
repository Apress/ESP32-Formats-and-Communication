/*******************************************************************************
 * Sketch name: Internet radio minimal sketch
 * Description: One station radio with M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>
#include <ssid_password.h>
#include <Audio.h>
Audio audio;

void setup()
{
  M5.begin();
  M5.Axp.SetSpkEnable(true);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (!WiFi.isConnected()) delay(500);
  audio.setPinout(12, 0, 2);
  audio.setVolume(12);
  delay(500);
  audio.connecttohost("media-ice.musicradio.com:80/ClassicFMMP3");
}

void loop()
{
  audio.loop();
}

void audio_showstreamtitle(const char *info)
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor (0, 50);
  M5.Lcd.print(String(info));
}
