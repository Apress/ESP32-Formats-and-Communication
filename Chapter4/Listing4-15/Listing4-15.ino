/*******************************************************************************
 * Sketch name: Synchronisation with Network Time Protocol
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appWwwTime()          // function to synchronise time with NTP
{
  int GMT = 0;            // non-GMT adjustment (secs)
  int daylight = 3600;          // daylight saving time (secs)
  tft->fillScreen(TFT_BLACK);       // clear screen
  tft->drawBitmap(0, 0, clockImage, 32, 32, TFT_WHITE); // draw image
  tft->setCursor(50, 10);
  tft->print("connecting");
  int wifiOK = WiFiconnect();       // connect to Wi-Fi network
  if(wifiOK == 0) return;       // exit if no connection
  configTime(GMT, daylight, "uk.pool.ntp.org");   // built-in esp32 function
  struct tm tnow;           // NTP data as structure
  if (!getLocalTime(&tnow)) tft->drawString("Error with time", 5, 30, 1);
  else
  {
    tft->fillRect(50, 10, 210, 110, TFT_BLACK);
    tft->setCursor(50, 10);
    tft->print("connected");
    for (int i=0; i<5; i++)       // display time for five seconds
    {
      tft->fillScreen(TFT_BLACK);
      tft->drawBitmap(0, 0, clockImage, 32, 32, TFT_WHITE);
      getLocalTime(&tnow);        // time from NTP
      tft->setCursor(20, 60);
      tft->print(&tnow, "%A \n");     // day of week
      tft->setCursor(20, 100);
      tft->print(&tnow, "%d %B %Y \n");   // date month year
      tft->setCursor(20, 140);
      tft->print(&tnow, "%H:%M:%S \n");   // hour : minute : second
      delay(1000);
    }               // set watch time to NTP time
    ttgo->rtc->setDateTime(tnow.tm_year, tnow.tm_mon + 1,
       tnow.tm_mday, tnow.tm_hour, tnow.tm_min, tnow.tm_sec);
  }
  WiFi.disconnect(true);        // disconnect Wi-Fi
  WiFi.mode(WIFI_OFF);
}

int WiFiconnect()         // function to connect to Wi-Fi network
{
  int wifiOK = 1;         // initial Wi-Fi connection indicator
  int count = 0;
  tft->setCursor(50, 120);
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED && wifiOK == 1)
  {
    delay(500);
    count++;            // number of connection attempts
    tft->print(".");
    if(count > 9)           // attempted connection > 5 sec
    {
       tft->setCursor(50, 160);
       tft->print("no connection");
       delay(1000);         // time for user to read message
       WiFi.disconnect(true);       // disconnect Wi-Fi
       WiFi.mode(WIFI_OFF);
       wifiOK = 0;        // update Wi-Fi connection indicator
    }
  }
  return wifiOK;        // pass variable to appWwwTime function
}
