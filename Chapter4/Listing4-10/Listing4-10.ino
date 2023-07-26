/*******************************************************************************
 * Sketch name: GPS information
 * Description: Display speed, altitude and distance from start
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appGPS()          // function to display GPS information
{
  double startLat, startLon;          // start position
  double distance, course;
  unsigned long last = 0;
  int value, startFlag = 0;
  tft->fillScreen(TFT_BLACK);         // clear touchscreen
  tft->drawBitmap(0, 0, GPSimage, 32, 32, TFT_WHITE);   //display GPS icon
  tft->setTextSize(1);            // reduce text size to one
  tft->setTextColor(TFT_YELLOW);
  tft->drawString("GPS", 90, 0, 4);       // font size 4
  int16_t x, y;
  while (!ttgo->getTouch(x, y))       // wait for touch
  {
    ttgo->gpsHandler();           // handle GPS signals
    if (gps->location.isUpdated())        // change in GPS location
    {
      if(startFlag == 1)        // start position already defined
      {              // distance and course to start position
        distance = TinyGPSPlus::distanceBetween(gps->location.lat(),
                   gps->location.lng(), startLat, startLon);
        course = TinyGPSPlus::courseTo(gps->location.lat(),
                 gps->location.lng(), startLat, startLon);
        tft->fillRect(120, 40, 100, 100, TFT_BLACK);  // "delete" previous values
        tft->setTextColor(TFT_WHITE);
        tft->drawString("Speed km/h", 0, 50, 4);
        value = round(gps->speed.kmph());     // display speed (km/h)
        tft->drawNumber(value, 150, 40, 6);
        tft->drawString("Altitude m", 0, 110, 4);
        value = round(gps->altitude.meters());    // display altitude (m)
        tft->drawNumber(value, 130, 100, 6);
        if(millis() - last > 5000)        // at 5 sec intervals
        {
          last = millis();          // update last time
          tft->setTextColor(TFT_GREEN);
          tft->fillRect(50, 160, 180, 70, TFT_BLACK);
          tft->drawString("Start km", 0, 170, 4);
          value = round(distance/1000);     // convert distance to km
          tft->drawNumber(value, 130, 160, 6);    // display distance
          tft->drawString("Course   "+String(TinyGPSPlus::
                          cardinal(course)), 0, 210, 4);    // display course
        }                 //   to start position
      }
      else if(startFlag == 0)         // GPS updated at
      {               //     start position
        startFlag = 1;
        startLat = gps->location.lat();
        startLon = gps->location.lng();
      }
    }
  }
  while (ttgo->getTouch(x, y)) {}       // wait for touch release
}
