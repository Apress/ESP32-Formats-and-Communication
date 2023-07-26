/*******************************************************************************
 * Sketch name: Memory storage
 * Description: Store an array and tect in PROGMEM
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

const int lookup[] PROGMEM = {2047,2402,2747,3071,3363};
const char text1[] PROGMEM = "ab12";
const char text2[] PROGMEM = "abc123";
const char text3[] PROGMEM = "abcd1234";
PGM_P const text[] PROGMEM = {text1,text2,text3};
int value;
char c;
char buffer[9];

void setup()
{
  Serial.begin(115200);
  for (int i=0; i<5; i++) // contents of an array with integers
  {
    value = pgm_read_word(lookup+i);
    Serial.println(value);
  }
  for (int i=0; i<strlen_P(text2); i++)
  {                     // contents of an array with characters
    c = pgm_read_byte(text2+i);
    Serial.print(c);
  }
  Serial.println();
  for (int i=0; i<3; i++)  // combined contents of arrays 
  {
    strcpy_P(buffer,(PGM_P)pgm_read_dword(& text[i]));
    Serial.println(buffer);
  }
//pointers on the ESP32 are 4 bytes (32 bits)
//pgm_read_word only returns a 16 bit number
//pgm_read_dword returns a 32 bit pointer
}

void loop()
{}
