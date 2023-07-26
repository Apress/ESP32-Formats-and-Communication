/*******************************************************************************
 * Sketch name: MP3 files on SD card and I2C decoder
 * Description: Play MP3 files with ESP32 DEVKIT DOIT
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S audio
 ******************************************************************************/

#include <Audio.h>          // include Audio and
#include <SD.h>           // SD libraries
Audio audio;            // associate audio with library
File SDdir;             // associate SDdir with SD library
int CS = 5;             // SD Card pins
int MOSIpin = 23;           // MOSI, MISO and SCK
int MISOpin = 19;           //   are reserved library terms
int SCKpin = 18;
int DIN = 26;             // define decoder pins for
int BCK = 27;           //   data input, audio bit clock and
int LRCK = 25;            //   the word select clock
unsigned long playTime = 0;
int play = 0;           // indicate track playing

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  SPI.begin(SCKpin, MISOpin, MOSIpin);    // initialise SPI
  SD.begin(CS);           // and SD
  SDdir = SD.open("/");         // open SD directory
  audio.setPinout(BCK, LRCK, DIN);      // initialise decoder
  audio.setVolume(15);          // set volume level: 0 to 21
  playTrack();            // play first track on SD card
}

void playTrack()          // function to play tracks on SD card
{
  play = 0;
  while(play == 0)          // while track not playing
  {
    File file =  SDdir.openNextFile();    // open next file on SD card
    if (file == true)         // found a file
    {
      if (!file.isDirectory())      // file is not a directory
      {
        Serial.print("playing ");Serial.println(file.name());
        audio.connecttoSD(file.name());   // play track
        playTime = millis();        // time track started playing
        play = 1;           // track playing
      }
    }
    else SDdir.rewindDirectory();     // rewind SD card directory
    file.close();
  }
}

void loop()
{
  audio.loop();
//  if(millis() - playTime > 10000) playTrack();  // play 10s of each track
}

void audio_eof_mp3(const char *info)    // action at end of file
{
  playTrack();            // call playTrack function
}
