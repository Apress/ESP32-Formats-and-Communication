/*******************************************************************************
 * Sketch name: Strings and characters
 * Description: Comparison methods
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

String a = "abcd";          // string
char b[] = "hijk";          // character array
const char * c = "rstu";        // pointer to character array
const char * d = "wxyz";

void setup()
{
  Serial.begin(115200);
  Serial.printf("pointer and string %d \n", strcmp(c, "abcd"));
  Serial.printf("pointer and c_str %d \n", strcmp(c, a.c_str()));
  Serial.printf("pointer and char %d \n", strcmp(c, b));
  Serial.printf("two pointers %d \n", strcmp(c, d));
  char charA[100];
  a.toCharArray(charA, a.length());      // convert string to character array
  Serial.printf("two char %d \n", strcmp(charA, b));
  String strB = String(b);        // convert character array to string
  Serial.printf("two c_str %d \n", strcmp(a.c_str(), strB.c_str()));
  Serial.printf("two strings %d \n", strcmp("abcd", "1234"));
}

void loop()
{}
