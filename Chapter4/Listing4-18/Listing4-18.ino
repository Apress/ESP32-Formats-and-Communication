/*******************************************************************************
 * Sketch name: Timer sketch
 * Description: Count-up and count-down timer
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appTimer()
{
  String ms, mmss[] = {"0", "0", ":", "0", "0"};  // minutes and seconds
  unsigned long last = 0;
  int s, m, count;
  int row, col, key = 0, pos = 0;
  tft->fillScreen(TFT_BLACK);
  printTime(pos, mmss);         // display timer time
  tft->setTextColor(TFT_ORANGE);
  for (int r=0; r<3; r++)       // draw buttons with numbers
  {               //     1 2 3
    for (int c=0; c<3; c++)       //     4 5 6
    {               //     7 8 9
      tft->fillRoundRect(c*80, r*50+35, 75, 45, 6, TFT_DARKCYAN);
      tft->drawNumber(r*3+c+1, c*80+30, r*50+40, 2);
    }
  }
  tft->fillRoundRect(0, 185, 75, 45, 6, TFT_DARKCYAN);
  tft->drawNumber(0, 30, 192, 2);     // button with zero
  tft->fillRoundRect(120, 200, 118, 35, 6, TFT_WHITE);
  tft->setTextColor(TFT_BLACK);
  tft->setCursor(155, 210);       // draw button to exit function
  tft->print("DONE");
  int16_t x, y;
  while(key < 11)           // key = 11/12 is DONE 
  {
    while(!ttgo->getTouch(x, y)) {}     // do nothing waiting for touch
    while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
    col = int(x/80) + 1;        // button in column number COL
    row = int((y-35)/50) + 1;       // button in row number ROW
    key = (row-1)*3+col;        // number on pressed button
    if(key == 10) key = 0;
    if(key < 11) mmss[pos] = key;     // position of digit in mm:ss
    pos++;              // move to next position
    if(pos > 4) pos = 0;        // return to first position
    if(pos == 2) pos++;         // pos(2) is the colon on mm:ss
    printTime(pos, mmss);
  }
  m = 10*mmss[0].toInt() + mmss[1].toInt(); // minutes and seconds
  s = 10*mmss[3].toInt() + mmss[4].toInt();
  if(m+s == 0) count = 1;       // time set to zero so count up
  else count = -1;          // otherwise count down
  tft->fillScreen(TFT_BLACK);       // clear screen
  tft->fillRoundRect(120, 200, 118, 35, 6, TFT_WHITE);
  tft->setTextColor(TFT_BLACK);
  tft->setCursor(145, 210);
  tft->print("FINISH");         // draw button to exit function
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextSize(1);          // need text size 1 for font 7
  while (!ttgo->getTouch(x, y))     // wait for touch
  {
    if(millis() - last > 1000)      // update timer every second
    {
      last = millis();
      ms = String(m) +":";        // display time mm:ss
      if(m < 10) ms = "0" + ms;
      if(s < 10) ms = ms + "0" + String(s); // include leading zeros
      else ms = ms + String(s);
      tft->drawString(ms, 50, 50, 7);   // font 7 for display of digits
      s = s+count;          // update seconds
      if(s > 59)            // minutes increased
      {
        s = 0;            // seconds to zero
        m = m+1;            // increment minutes
      }
      if(s < 0)           // minutes decreased
      {
        s = 59;           // seconds to 59
        m = m-1;            // decrement minutes
      }
      if(m < 0)           // timer reached 00:00
      {
        drv->selectLibrary(1);      // activate vibration motor
        drv->setMode(DRV2605_MODE_INTTRIG);
        drv->setWaveform(0, 70);      // vibrat. pattern 70 in position 0
        drv->setWaveform(1, 0);     // end of waveforms
        drv->go();          // activate vibration motor
        m = 0;
        s = 0;          // stay in loop until screen pressed
      }
    }
  }
  while (ttgo->getTouch(x, y)) {}     // wait for touch release
}

void printTime(int pos, String timeMS[5]) // function to display time (mm:ss)
{
  tft->setCursor(85, 2);
  tft->setTextSize(3);          // increase text size to 3
  for (int i=0; i<5; i++)       // display mm:ss
  {               // colours to overwrite
    tft->setTextColor(TFT_WHITE, TFT_BLACK);  //      previous values
    if(i == pos) tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    tft->print(timeMS[i]);
  }
  tft->setTextSize(2);          // reset to text size 2
}
