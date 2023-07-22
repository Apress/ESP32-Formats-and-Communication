/*******************************************************************************
 * Sketch name: Battery charge sketch
 * Description: Display barrety charge/dischage current, voltage and charged%
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appBattery()      // function to display battery voltages and current
{
  tft->fillScreen(TFT_BLACK);
  tft->drawBitmap(0, 0, batteryImage, 32, 32, TFT_WHITE);
  tft->setTextColor(TFT_YELLOW);
  tft->setCursor(60, 10);
  tft->print("Battery");
  tft->setTextColor(TFT_GREEN);
  unsigned long last = 0;
  int16_t x, y;
  while (!ttgo->getTouch(x, y))       // wait for touch
  {
    if(millis() - last > 5000)        // display every 5 seconds
    {
      last = millis();            // update display time
      float chrg_p = power->getBattChargeCurrent(); // charging current
      float chrg_n = power->getBattDischargeCurrent();// discharging current
      float batt_v = power->getBattVoltage();   // battery voltage
      int   batt_p = power->getBattPercentage();  // battery %
      tft->fillRect(40, 60, 180, 120, TFT_BLACK);
      tft->setCursor(40, 60);
      tft->printf("+chrg %.0fmA \n", chrg_p);
      tft->setCursor(40, 90);
      tft->printf("-chrg %.0fmA \n", chrg_n);   // display battery state
      tft->setCursor(40, 120);
      tft->printf("Batt  %0.fmV \n", batt_v);
      tft->setCursor(40, 150);
      tft->printf("Batt  %d%% \n", batt_p);   // double %% displays %
    }
  }
  while (ttgo->getTouch(x, y)) {}       // wait for touch release
}
