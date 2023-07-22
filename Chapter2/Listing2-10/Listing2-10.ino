/*******************************************************************************
 * Sketch name: File structure on micro-SD card
 * Description: List directories and files within directories
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

void listDir(String dir, int level)
{
  File SDdir = SD.open(dir);
  File file = SDdir.openNextFile();    // start at top level of SD card
  while(file)
  {
    if(file.isDirectory())      // when file is a directory
    {             //    check files within a directory
      Serial.printf("DIR %s \n", file.name());
      if(level) listDir(file.path(), level-1);
    }
              // when file is not a directory, display file name and size
    else Serial.printf("   %s size %dKB \n", file.name(),file.size()/1024);
    }
    file = SDdir.openNextFile();    // next file
  }
}
