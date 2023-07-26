/*******************************************************************************
 * Sketch name: Screen brightness
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

int appBright()          // function to change screen brightness
{
  int row, col, key = 0;          // brightness levels
  int level = 0, levels[] = {8, 16, 32, 64, 128, 192, 255};
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE);
  tft->drawString("ABCD1234", 60, 2, 2);
  tft->setTextColor(TFT_ORANGE);
  for (int r=0; r<2; r++)       // draw buttons with numbers
  {               //     1 2 3
    for (int c=0; c<3; c++)       //     4 5 6
    {
      tft->fillRoundRect(c*80, r*50+35, 75, 45, 6, TFT_DARKCYAN);
      tft->drawNumber(r*3+c+1, c*80+30, r*50+40, 2);
    }
  }
  tft->fillRoundRect(120, 200, 118, 35, 6, TFT_WHITE);
  tft->setTextColor(TFT_BLACK);
  tft->setCursor(155, 210);       // draw button to exit function
  tft->print("DONE");
  int16_t x, y;
  while(key < 7)            // key = 11 or 12 is "DONE"
  {
    while(!ttgo->getTouch(x, y)) {}     // do nothing waiting for touch
    while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
    col = int(x/80) + 1;        // button in column number COL
    row = int((y-35)/50) + 1;       // button in row number ROW
    key = (row-1)*3+col;        // number on pressed button
    if(key < 7) level = key;
    ttgo->setBrightness(levels[level]);   // brightness levels 0 to 6
  }
  return level;           // return brightness level
}               //      to main sketch
