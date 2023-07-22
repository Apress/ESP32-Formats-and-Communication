/*******************************************************************************
 * Sketch name: Default display screen
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void displayTime(int update, int bright)      // default display screen
{
  RTC_Date tnow = ttgo->rtc->getDateTime();   // update RTC time
  hh = tnow.hour;
  mm = tnow.minute;
  ss = tnow.second;
  if (update)             // update display screen
  {
    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);          // reduce text size to 1
    String hhmm = String(hh) +":";        // display time
    if(hh < 10) hhmm = "0" + hhmm;        //     with leading zeros
    if(mm < 10) hhmm = hhmm + "0" + String(mm);
    else hhmm = hhmm + String(mm);
    tft->drawString(hhmm, 45, 35, 7);     // font 7 is 7-segment

    String months[] = {"","Jan","Feb","Mar","Apr","May","June",
                          "July","Aug","Sept","Oct","Nov","Dec"};
    String wkdays[] = {"Sun","Mon","Tues","Wed","Thur","Fri","Sat"};
    int wkday = ttgo->rtc->getDayOfWeek(tnow.day, tnow.month, tnow.year);
    tft->setTextColor(TFT_GREENYELLOW);
    tft->drawString(wkdays[wkday], 20, 110, 4);
    tft->drawNumber(tnow.day, 85, 100, 6);    // weekday, date & month
    tft->drawString(months[tnow.month], 160, 110, 4);

    tft->fillRect( 65, 160, 40, 10, TFT_YELLOW);  // three colour bars for
    tft->fillRect(105, 160, 40, 10, TFT_DARKGREEN); //      microcontroller temp
    tft->fillRect(145, 160, 40, 10, TFT_RED);
    tft->drawString("Temp", 30, 157, 2);
    float temp = power->getTemp();        // microcontroller temp
    tft->drawNumber(round(temp), 118, 157, 2);    // display temperature
    int x = 35+3*round(temp);         // range 10 to 50C
    tft->fillTriangle(x, 175, x-5, 195, x+5, 195, TFT_WHITE);

    tft->setTextColor(TFT_GREEN);       // display battery percent,
    int batt_p = power->getBattPercentage();    // screen brightness level
    tft->drawString("Battery  "+ String(batt_p) + "%", 0, 2, 2);
    tft->drawString("Bright  " + String(bright), 165, 2, 2);
    tft->drawString("Steps " + String(bma->getCounter()), 0, 220, 2);
  tft->fillRoundRect(119, 210, 120, 29, 15, TFT_DARKCYAN);
  }
  int sval = ss/10;           // seconds (tens)
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->drawNumber(sval, 100, 213, 4);
  sval = ss % 10;             // seconds (units)
  if (sval > 0) tft->fillRect(126 + sval * 10, 217, 6, 15, TFT_ORANGE);
  else tft->fillRoundRect(119, 210, 120, 29, 15, TFT_DARKCYAN);
}                 // seconds animation
