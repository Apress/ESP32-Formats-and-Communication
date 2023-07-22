/*******************************************************************************
 * Sketch name: GPS location
 * Description: Display latitude, longitude, altitude, spped and satellites
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appGPSout()          // function to display GPS location
{
  tft->fillScreen(TFT_BLACK);         //display GPS icon
  tft->drawBitmap(0, 0, GPSimage, 32, 32, TFT_WHITE);
  tft->setTextSize(2);            // change text size to 2
  tft->setTextColor(TFT_YELLOW);
  tft->setCursor(80, 10);
  tft->print("GPS data");
  tft->setTextColor(TFT_GREEN, TFT_BLACK);
  unsigned long last, lag;
  int16_t x, y;
  last = millis();
  while (!ttgo->getTouch(x, y))       // wait for touch
  {
    ttgo->gpsHandler();           // handle GPS signals
    if (gps->time.isUpdated())        // updated GPS time
    {
      tft->setCursor(0, 40);        // leading zeros for 2 digits
      tft->printf("tim %02d:%02d:%02d \n", gps->time.hour(),
                   gps->time.minute(),gps->time.second());
      lag = round((millis() - last)/1000.0);    // lag since last
      tft->setCursor(0, 220);         //   updated location
      if(lag > 0) tft->printf("lag %d   \n", lag);
    }
    if (gps->altitude.isUpdated())        // updated altitude
    {
      tft->setCursor(0, 70);
      tft->printf("alt %.1f m  \n", gps->altitude.meters());
    }
    if (gps->speed.isUpdated())       // updated speed
    {
      tft->setCursor(0, 100);         // display speed in mps
      tft->printf("spd %.1f mps  \n", gps->speed.mps());
    }
    if (gps->satellites.isUpdated())    // updated number of satellites
    {
      tft->setCursor(0, 130);
      tft->printf("sat %d  \n", gps->satellites.value());
    }
    if (gps->location.isUpdated())        // updated location
    {
      tft->setCursor(0, 160);
      tft->printf("lat %.5f \n", gps->location.lat());
      tft->setCursor(0, 190);
      tft->printf("lon %.5f \n", gps->location.lng());
      last = millis();            // update last time when
    }                 //    new location received
  }
  while (ttgo->getTouch(x, y)) {}       // wait for touch release
}
