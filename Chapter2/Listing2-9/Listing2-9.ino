/*******************************************************************************
 * Sketch name: Play MP3 files with M5Stack Core2
 * Description: Comprehensive internet radio with functions
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>
#include <Audio.h>
Audio audio;
int SD_CS = 4, SD_SCK = 18, SD_MISO = 38, SD_MOSI = 23;
int I2S_BCLK = 12, I2S_LRC = 0, I2S_DOUT = 2;
uint8_t cardType;
uint64_t cardSize;
String temp, filenames[20];
int MP3play = 0, vol = 10, Nfiles = 0, Nmp3 = 0;
float batt;
 
void setup()
{
  M5.begin();
  M5.Axp.SetSpkEnable(true);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,0);
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  SD.begin(SD_CS);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(vol);
  getFiles();
//  Serial.println("list of MP3 files");
//  for (int i=0; i<Nmp3; i++) Serial.println(filenames[i]);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.fillScreen(BLACK);
  battery();
  temp = filenames[0];
  audio.connecttoFS(SD, temp.c_str());
//  Serial.printf("track %d %s \n", 0, temp.c_str());
}

void getFiles()
{
  M5.Lcd.setTextColor(YELLOW);
  if(!SD.begin())
  {
    M5.Lcd.println("Card Mount Failed");
    return;
  }
  cardType = SD.cardType();
  if(cardType == CARD_NONE)
  {
    M5.Lcd.println("No SD_MMC card attached");
    return;
  }
  M5.Lcd.print("SD_MMC Card Type: ");
  if(cardType == CARD_MMC) M5.Lcd.println("MMC");
  else if(cardType == CARD_SD) M5.Lcd.println("SDSC");
  else if(cardType == CARD_SDHC) M5.Lcd.println("SDHC");
  else M5.Lcd.println("UNKNOWN");
  cardSize = SD.cardSize() / (1024 * 1024);
  M5.Lcd.printf("\nSD_MMC Card Size %lluMB\n", cardSize);  // blank lines between display line
//  listDir("/", 0);   // only access "top level" files
  listDir("/", 1);   // will list files within a directory
  M5.Lcd.printf("\nFiles %d \n", Nfiles);
  M5.Lcd.printf("\nMP3 files %d \n", Nmp3);
  delay(3000);   // time to read file information
}

void listDir(String dir, int level)    // function to list files in an SD directory
{
  File SDdir = SD.open(dir);        // open SD directory
  File file = SDdir.openNextFile();     // open file on SD card
  while (file)            // located a file
  {
    if (file.isDirectory())
    {
      Serial.printf("DIR %s \n", file.name());
      if(level) listDir(file.path(), level-1);  // move down a directory
    }
    else              // when file is not a directory
    {
      Serial.printf("   file %s \n", file.name());
      Nfiles++;           // increment number of files
      temp = file.name();
      if(temp.indexOf(".mp3") > 2)      // check if filename includes mp3
      {
        filenames[Nmp3] = file.path();    // add MP3 file path to array
        Nmp3++;           // increment number of MP3 files
      }
    }
    file = SDdir.openNextFile();      // open next file
  }
}

void loop()
{
  audio.loop();
  if (M5.BtnA.wasPressed()) // isPressed: button state last time button read
  {                         // wasPressed: returns 1 when button pressed
    vol++;
    if(vol > 21) vol = 0;
    audio.setVolume(vol);
    battery();
  }
  if (M5.BtnB.wasPressed())
  {
    MP3play++;
    if(MP3play > Nmp3-1) MP3play = 0;
    changeTrack();
  }
  if (M5.BtnC.wasPressed())
  {
    MP3play--;
    if(MP3play < 0) MP3play = Nmp3-1;
    changeTrack();
  } 
  M5.update();
}

void battery()
{
  if(vol < 10) temp = "Volume  "+ String(vol);
  else temp = "Volume " + String(vol);
  M5.Lcd.setTextColor(YELLOW, BLACK);  
  M5.Lcd.drawString(temp, 0, 220);
  batt = M5.Axp.GetBatVoltage();
  temp = "Battery "+ String(batt,1);  // 1DP as batt is float
  M5.Lcd.drawString(temp, 160, 220);
  M5.Lcd.setTextColor(GREEN, BLACK);
}

void audio_id3data(const char *info)  //MP3 file data
{
  String chk = String(info);  // convert char array to string    
  if(!(chk.startsWith("Content") || chk.startsWith("Track") ||    // exclude some details
       chk.startsWith("Setting") || chk.startsWith("Publisher") ||
       chk.startsWith("Band")))
  splitText(chk);
  Serial.print("data ");Serial.println(chk);
}

void audio_eof_mp3(const char *info)  //end of file info contains filename
{
  Serial.print("eof ");Serial.println(info);
  MP3play++;             // at end of file, play next file
  if(MP3play > Nmp3-1) MP3play = 0;
  changeTrack();
}

void changeTrack()
{
  audio.stopSong();
  temp = filenames[MP3play];
  audio.connecttoFS(SD, temp.c_str());
//  Serial.printf("track %d %s \n", MP3play, temp.c_str());
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,0);
  battery();
}

void splitText(String text)
{
  int line = 25, spaces[50], Nspaces = 0, firstC = 0, lastC = 1;
  String str = text +" ";  // set last space at end of text
  int len = str.length();
  spaces[0] = -1;
  for (int i=0; i<50; i++)
  {
    if(spaces[i]+1 < len)  // not at end of text
    {
      spaces[i+1] = str.indexOf(" ",spaces[i]+1); // locate next space
      Nspaces++;
    }
    else i = 50; // found all spaces
  }
  for (int i=1; i<Nspaces+1; i++)
  {
    if(spaces[i]-lastC > line)  // current space past line length
    {
      lastC = spaces[i-1]; // finish position set to previous space
      M5.Lcd.println(str.substring(firstC, lastC)); // display text between spaces
      firstC = lastC+1; // finish position set to previous space+1
    }
  }
  M5.Lcd.println(str.substring(firstC, len-1));  // display from last space to end of text
  M5.Lcd.println();
}

void audio_info(const char *info)
{
  Serial.print("info ");Serial.println(info);
}
