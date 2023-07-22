/*******************************************************************************
 * Sketch name: Microcontroller temperature sketch
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appTemp()          // function to display IC temperature
{ 
    float temp = power->getTemp();      // get microcontroller temperature
    tft->fillScreen(TFT_BLACK);     // clear screen
    tft->setSwapBytes(true);        // required to retain image colour
    tft->pushImage(40, 0, 163, 240, image565);    // display colour image
    tft->setTextColor(TFT_YELLOW, TFT_BLACK);   //     as background
    tft->setCursor(60, 195);
    tft->printf("Temp: %.1f \n", temp);     // display temperature
  
  int16_t x, y;
  while(!ttgo->getTouch(x, y)) {}     // do nothing waiting for touch
  while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
}
