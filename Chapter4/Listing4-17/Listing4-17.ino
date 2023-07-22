/*******************************************************************************
 * Sketch name: Step counter and distance measure
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appStep()        // function for step counter and distance measure
{
  Acfg cfg;             // accel parameter structure
  cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;    // output data rate in Hz
  cfg.range = BMA4_ACCEL_RANGE_2G;      // gravity (g) range
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;   // bandwidth parameter
  cfg.perf_mode = BMA4_CONTINUOUS_MODE;   // performance mode
  bma->accelConfig(cfg);        // configure accelerometer
  bma->enableAccel();         // enable accelerometer
  bma->enableFeature(BMA423_STEP_CNTR, true); // enable step count feature
  bma->enableStepCountInterrupt();      // turn on step count interrupt
  int startFlag = 0;
  unsigned long lastGPS;
  static double lastLat, lastLon;     // retain last position
  static float distTotal;       // retain total distance
  float dist;
  tft->fillScreen(TFT_BLACK);           // clear screen
  tft->drawBitmap(0, 0, stepImage, 32, 32, TFT_WHITE);  // draw icon
  tft->setCursor(40, 20);
  tft->setTextColor(TFT_YELLOW);
  tft->print("Step counter");
  printStep(distTotal);       // display steps, distance & satellites
  tft->fillRoundRect(120, 200, 118, 35, 6, TFT_WHITE);
  tft->setTextColor(TFT_BLACK);
  tft->setCursor(155, 210);
  tft->print("RESET");

  int16_t x, y;
  while (!ttgo->getTouch(x, y))     // functions as a loop
  {
    if (irq)               // accelerometer interrupt triggered
    {
      irq = 0;            // reset accelerometer interrupt
      while(bma->readInterrupt() != 1) {}   // wait for interrupt to reset
      if (bma->isStepCounter()) printStep(distTotal); // if step counter set,
    }                 //     update display
    ttgo->gpsHandler();         // handle GPS signals
    if (gps->location.isUpdated())      // updated location
    {
      if(startFlag == 1)        // start position already defined
      {             // distance to previous position
        dist = TinyGPSPlus::distanceBetween(gps->location.lat(),
               gps->location.lng(), lastLat, lastLong);
        if((millis() - lastGPS > 15000) && dist > 8)
        {             // minimum interval and distance
          lastGPS = millis();       // update timer
          distTotal = distTotal + dist;   // cumulative distance
          lastLat = gps->location.lat();    // update latitude and longitude
          lastLon = gps->location.lng();
        }
      }
      else if(startFlag == 0)         // GPS updated at
      {               //     start position
        startFlag = 1;
        lastLat = gps->location.lat();
        lastLon = gps->location.lng();
      }
    }
  }
  while (ttgo->getTouch(x, y)) {}     // wait for touch release
  if(x>155 && y>210)          // reset button pressed
  {
    bma->resetStepCounter();        // reset steps and distance
    totalDist = 0;
    printStep(totalDist);       // display reset values
    delay(1000);            // time for user to read update
  }
}

void printStep(float distVal)   // function to display steps, distance & satellites
{
  uint32_t step = bma->getCounter();    // get step data from register
  tft->setTextColor(TFT_GREEN);
  tft->fillRect(40, 70, 180, 100, TFT_BLACK);
  tft->setCursor(40, 70);
  tft->printf("Steps %d \n", step);     // display steps, distance
  tft->setCursor(40, 110);        //      and satellite number
  tft->print("Distance ");tft->print(round(distVal), 0);
  tft->setCursor(40, 150);
  tft->printf("Satellites %d \n", gps->satellites.value());
}
