/*******************************************************************************
 * Sketch name: Menu sktech
 * Description: Generate menu to select an application
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

// 1) Update maxApp to the total number of apps
// 2) Update appName[] to add the title of the app
// 3) Update getIcon, add a case to the switch statement for an icon
// 4) In TTGOwatch tab, add a case to the switch statement to call the app

const int maxApp = 12;          // number of apps plus one
String appTitle[maxApp] =       // application titles in menu
{"","Bright","GPS","Battery","Bluetooth","InfraRed","Step count",
    "Temp","Timer","Weather","www time","GPSout","Satellites"};
int menu()              // function to update list
{               //     when screen touched
  int appN = 0;           // initial list starts with blank
  int exit = 0;           // stay in menu until app selected
  list(appN);           // call function to display app list
  int16_t x, y;
  while (!exit)           // while app not selected
  {
    while (!ttgo->getTouch(x, y)) {}    // do nothing waiting for touch
    while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
    {
      if (y > 80 && y < 160) exit = 1;    // middle app on list selected
      appN = appN - int(y/80) + 1;      // move to next or previous app
      if (appN == maxApp) appN = 0;     // list starts with blank
      else if (appN < 0) appN = maxApp - 1; // list starts with last app
      list(appN);           // call function to display app list
    }
  }
  tft->setTextSize(2);    // text size 2 as returning to an app not to displayTime
  tft->setTextColor(TFT_GREEN);     // is this needed ??
  return appN;
}

void list(int N)            // function to display app titles
{
  tft->fillScreen(TFT_DARKCYAN);
  tft->fillRect(0, 80, 240, 80, TFT_BLACK); // rectangle in centre of screen
  tft->setTextSize(3);          // large text for middle app
  tft->setTextColor(TFT_RED);
  tft->setCursor(50, 110);
  tft->println(appTitle[N]);        // display app title and draw icon
  if(N > 0) tft->drawBitmap(0, 100, getIcon(N), 32, 32, TFT_WHITE);
  tft->setTextSize(2);          // small text for top & bottom app
  tft->setTextColor(TFT_ORANGE);
  int n = N-1;            // previous app
  if (n < 0) n = maxApp - 1;
  tft->setCursor(50, 30);       // display app title at top of screen
  tft->print(appTitle[n]);        //     with icon
  if(n>0) tft->drawBitmap(0, 20, getIcon(n), 32, 32, TFT_WHITE);
  n = N+1;              // next app
  if (n > maxApp - 1) n = 0;
  tft->setCursor(50, 190);        // app title at bottom of screen
  tft->println(appTitle[n]);
  if(n>0) tft->drawBitmap(0, 180, getIcon(n), 32, 32, TFT_WHITE);
}

const unsigned char * getIcon(int N)    // switch-case for app icon
{
  switch (N)
  {
    case 1: return bulbImage; break;    // appBright, icon not used in app
    case 2: return GPSimage; break;
    case 3: return batteryImage; break;
    case 4: return BTimage; break;      // appBlue
    case 5: return IRimage; break;      // appInfraRed, icon not used
    case 6: return stepImage; break;
    case 7: return tempImage; break;    // icon not used in app
    case 8: return clockImage; break;   // appTimer, icon not used in app
    case 9: return weatherImage; break;
    case 10: return clockImage; break;    // appWwwTime
    case 11: return GPSimage; break; // appGPSout
    case 12: return GPSimage; break; // appSatellite
    default: break;
  }
}
