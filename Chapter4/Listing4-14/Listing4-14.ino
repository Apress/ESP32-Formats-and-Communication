/*******************************************************************************
 * Sketch name: Infrared signalling
 * Description: Example with a Sony device
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appSonyIR()          // function to transmit infrared signals
{
  irsend.begin();           // initialise library object
  int row, col, key = 0;
  String button[3] = {"Start", " ON", "Stop"};  // button text
  uint16_t raw[25];         // array for signal data
  uint16_t start[] = {4, 1,2,1,1, 2,2,1,2, 1,1,1,2};  // LOW pulses for 0x4D1
  uint16_t pauseTime = 25200;       // interval (s) between signals
  tft->fillScreen(TFT_BLACK);       // clear screen
  tft->setTextColor(TFT_ORANGE);
  int r = 0;            // one row in the example
  for (int c=0; c<3; c++)       // draw buttons with text
  {               //      from button[ ] array
    tft->fillRoundRect(c*80, r*50+35, 75, 45, 6, TFT_DARKCYAN);
    tft->drawString(button[c], c*80+5, r*50+40, 2);
  }
  tft->fillRoundRect(120, 200, 118, 35, 6, TFT_WHITE);
  tft->setTextColor(TFT_BLACK);
  tft->setCursor(155, 210);       // button to exit function
  tft->print("DONE");
  int16_t x, y;
  while(key < 10)           // key = 11/12 is "DONE" 
  {
    while(!ttgo->getTouch(x, y)) {}     // do nothing waiting for touch
    while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
    col = int(x/80) + 1;        // button in column number COL
    row = int((y-35)/50) + 1;       // button in row number ROW
    key = (row-1)*3+col;        // button number
    if(key < 4)
    {
           if(key == 1) irsend.sendSony(0x4D1, 12, 3);    // signal, bits, repeats
      else if(key == 3) irsend.sendSony(0x1D1, 12, 3);
      else if(key == 2)
      {             // generate signal raw data
        for (int i=0; i<13; i++) raw[2*i] = start[i]*600; // LOW pulses
        for (int i=1; i<13; i++) raw[2*i-1] = 600;  // constant HIGH pulses
        for (int j=0; j<3; j++)     // transmit 3 signal repeats
        {
          irsend.sendRaw(raw, 25, 38);    // data, signal length, kHz
          delayMicroseconds(pauseTime);   // delay between signals
        }
      }
      key = 0;          // reset key to prevent repeat signals
    }
  }
}
