/*******************************************************************************
 * Sketch name: Internet raduio with ESP32 module
 * Description: One station radio with decoder
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <Audio.h>          // include Audio
#include <WiFi.h>           // and WiFi libraries
#include <ssid_password.h>        // file with Wi-Fi login details
Audio audio;            // associate audio with library
int DIN = 26;             // define decoder pins for
int BCK = 27;           //   data input, serial bit clock and
int LRCK = 25;            //   the word select clock
int volPin = 34;            // define potentiometer pin
int volume = 10, oldVol = 0;        // initial volume level
unsigned long lastTime = 0;
String str;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  audio.setPinout(BCK, LRCK, DIN);      // initialise decoder
  audio.setVolume(volume);        // set volume level: 0 to 21
  audio.connecttohost("media-ice.musicradio.com/ClassicFM");  // Classic FM
}

void loop()
{
  audio.loop();
  if(millis() - lastTime > 5000) getVolume(); // update volume every 5s
  if(str.startsWith("Request"))
  {
    audio.connecttohost("media-ice.musicradio.com/ClassicFM");  // Classic FM
    str = "";
  }
}

void getVolume()            // function to update volume
{
  volume = analogRead(volPin);      // read potentiometer value
  volume = constrain(volume, 620, 3100);    // constrain to linear region
  volume = map(volume, 620, 3100, 0, 21);   // map analog values to volume
  if(oldVol != volume)          // volume changed
  {
    audio.setVolume(volume);        // set volume
    Serial.printf("volume %d \n", volume);  // display new volume
    oldVol = volume;
  }
  lastTime = millis();          // update volume reading time
}

void audio_showstation(const char *info)    // display radio station name
{
  Serial.printf("station %s \n",info);
}
void audio_showstreamtitle(const char *info)  // display title of streamed track
{
  Serial.printf("streamtitle %s \n", info);
}
void audio_info(const char *info)
{
  str = info;
  if(str.startsWith("Connected") || str.startsWith("Request"))
     Serial.printf("info %s \n", info);
}
