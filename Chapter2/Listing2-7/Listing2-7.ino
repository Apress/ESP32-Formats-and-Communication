/*******************************************************************************
 * Sketch name: Function to parse text
 * Description: Split text by locating position of spaces
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

void splitText(String text)
{
  int line = 25, spaces[50], Nspaces = 0, firstC = 0, lastC = 1;
  String str = text + " ";        // add space at end of text
  int len = str.length();
  spaces[0] = -1;
  for (int i=0; i<50; i++)
  {
    if(spaces[i]+1 < len)         // not at end of text
    {
      spaces[i+1] = str.indexOf(" ",spaces[i]+1); // locate next space
      Nspaces++;              // number of spaces
    }
    else i = 50;              // found all spaces
  }
  for (int i=1; i<Nspaces+1; i++)       // for each space
  {
    if(spaces[i]-lastC > line)      // current space past line length
    {
      lastC = spaces[i-1];        // finish position set to next space
      M5.Lcd.println(str.substring(firstC, lastC));// display text between spaces
      firstC = lastC+1;           // start position set to 
    }                 //     previous space+1
  }
  M5.Lcd.println(str.substring(firstC, len-1));   // display from last space
}                 //    to end of text
