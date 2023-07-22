/*******************************************************************************
 * Sketch name: Write, read and append file in SPIFFS
 * Description: Uses LittleFS library for ESP32
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <LittleFS.h>          // include LittleFS library
String dirAname = "/dirA";
String dirBname = "/dirB";
String file1 = "/dirA/tstfile1.txt";   // structure /dir/ file
String file2 = "/dirA/tstfile2.txt";   // level       0    1
String file3 = "/dirB/tstfile3.txt";
unsigned long lag;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  if(LittleFS.begin()) Serial.println("LittleFS OK");
  createDir(dirAname);  // create directory
  createDir(dirBname);
  File file = LittleFS.open(file1, FILE_WRITE);  // open file to write
  file.println("ABC");          // add record with ABC
  file.println("123");          // instead of print("xxx\n")
  file.close();
  fileContent(file1);        // function to display file content
  dirContent(dirAname);          //    and directory content
  file = LittleFS.open(file1, FILE_APPEND);  // append to a file
  file.println("XYZ");
  file.close();
  LittleFS.rename(file1, file2);     // change filename not directory
  fileContent(file2);
//  if(LittleFS.exists(file2)) LittleFS.remove(file2);  // delete a file
  file = LittleFS.open(file1, FILE_WRITE);   // create a new file
  file.close();
  file = LittleFS.open(file3, FILE_WRITE);   // create a new file
  file.close();
  dirContent("/"); // list of directories
  dirContent(dirAname); // files in directory
  Serial.println("\nSPIFFS hierarchy");
  hierarchy("/", 1);  // hierarchy of SPIFFS
  lag = millis();
  LittleFS.format();          // delete all files in SPIFFS
  lag = millis() - lag;
  Serial.printf("\ntime to delete: %u \n", lag);
  dirContent("/");
}

void dirContent(String dname)     // function to display directory content
{
  File dir = LittleFS.open(dname);        // open directory
  Serial.printf("\n%s content\n", dir.name());
  File file = dir.openNextFile();       // open file in directory
  while(file)
  {                 // display file details
    Serial.printf("file %s size %d \n", file.name(),file.size());
    file = dir.openNextFile();
  }
}

void fileContent(String fname)    // function to display file content
{
  File file = LittleFS.open(fname, FILE_READ);    // open file and
  Serial.printf("\nFile content of %s \n", file.name());
  while(file.available()) Serial.write(file.read());  //    display file content
  file.close();
}

void createDir(String dirname)  // sorted list of filenames 
{
  if(LittleFS.mkdir(dirname)) printf("directory %s OK \n", dirname);
}

void hierarchy(String dirname, int level)
{
  File root = LittleFS.open(dirname);
  File file = root.openNextFile();
  while (file)
  {
    if(file.isDirectory()) // when file is a directory
    {             //    check files within directory
      Serial.printf("DIR content of %s \n", file.name());
      if(level) hierarchy(file.path(), level-1);
    }                   // display file name and size       
    else Serial.printf("  File %s size %d \n", file.name(),file.size());
    file = root.openNextFile();
  }
}

void loop()             // nothing in loop function
{}
