/*******************************************************************************
 * Sketch name: 
 * Description: SPIFFS versuion of Listing 10-8
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <SPIFFS.h>          // include SPIFFS library
String file1 = "/dirA/tstfile1.txt";   // structure /dir/ file
String file2 = "/dirA/tstfile2.txt";
String file3 = "/dirB/tstfile3.txt";
unsigned long lag;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  if(SPIFFS.begin()) Serial.println("SPIFFS OK");
  File file = SPIFFS.open(file1, FILE_WRITE);  // open file to write
  file.println("ABC");          // add record with ABC
  file.println("123");          // instead of print("xxx\n")
  file.close();
  fileContent(file1);        // function to display file content
  dirContent("/dirA");          //    and directory content
  file = SPIFFS.open(file1, FILE_APPEND);  // append to a file
  file.println("XYZ");
  file.close();
  SPIFFS.rename(file1, file2);     // change filename not directory
  fileContent(file2);
//  if(SPIFFS.exists(file2)) SPIFFS.remove(file2);  // delete a file
  file = SPIFFS.open(file1, FILE_WRITE);   // create new files
  file.close();
  file = SPIFFS.open(file3, FILE_WRITE);
  file.close();
  dirContent("/");  // effectively lists all files without "directory" prefix
  dirContent("/dirA"); // files in "dirA" directory
  lag = millis();
  SPIFFS.format();          // delete all files in SPIFFS
  lag = millis() - lag;
  Serial.printf("\ntime to delete: %u ms \n", lag);
  dirContent("/");
}

void dirContent(String dname)     // function to display directory content
{
  File dir = SPIFFS.open(dname);        // open directory
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
  File file = SPIFFS.open(fname, FILE_READ);    // open file and
  Serial.printf("\nFile content of %s \n", file.name());
  while(file.available()) Serial.write(file.read());  //    display file content
  file.close();
}

void loop()             // nothing in loop function
{}
