/*******************************************************************************
 * Sketch name: Analog watch face
 * Description: Display on 1.3" 240x240 pixel TFT ST7789 LCD
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite dial = TFT_eSprite(&tft);   // Sprite objects for dial
TFT_eSprite hourh = TFT_eSprite(&tft);    //    and for clock hands
TFT_eSprite minuteh = TFT_eSprite(&tft);
TFT_eSprite secondh = TFT_eSprite(&tft);

#include <WiFi.h>           // include Wi-Fi library
#include <ssid_password.h>        // file with logon details
#include <time.h>           // include time library
int GMT = 0, daylight = 3600;     // GMT and daylight saving offset (sec)
int hh, mm, ss, wd, dd, mn;
int posx, posy;
struct tm timeData;         // time structure
String day[] = {"Sun","Mon","Tues","Wed","Thur","Fri","Sat"};
String mon[] = {"Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"};
int width = tft.width(), height = TFT_HEIGHT; // define screen dimensions
int centX = width/2, centY = height/2;    // centre of screen position
int dialR = 95;           // clock face radius for white area
int Llong = 20, Lshort = 10;        // lengths of minute markers

void setup()
{
  WiFi.begin(ssid, password);       // initialise and connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  configTime(GMT, daylight, "uk.pool.ntp.org");   // NTP pool
  while (!getLocalTime(&timeData)) delay(500);
  WiFi.disconnect(true);        // disconnect Wi-Fi
  WiFi.mode(WIFI_OFF);
  
  tft.init();           // initial LCD screen
  drawDial();           // function to display clock face
  drawHour(11, 70, TFT_LIGHTGREY);      // function to create clock hands
  drawMinute(11, 90, TFT_LIGHTGREY);    //    width, length and colour
  drawSecond(3, 90, TFT_BLACK);       //    of clock hand
}

void loop()
{
  getLocalTime(&timeData);        // obtain current time and date
  hh = 30 * timeData.tm_hour + timeData.tm_min/2 + 180;
  mm = 6 * timeData.tm_min + 180;     // zero degrees at bottom of circle
  ss = 6 * timeData.tm_sec + 180;     // with screen connections at top
  wd = timeData.tm_wday;        // day of week
  dd = timeData.tm_mday;        // date
  mn = timeData.tm_mon;         // month
  drawDial();
  hourh.pushRotated(&dial, hh);     // position hour, minute and
  minuteh.pushRotated(&dial, mm);     //    second hands on clock face
  secondh.pushRotated(&dial, ss);
  dial.pushSprite(0,0);         // push clock Sprite
  delay(100);           // interval between screen updates
  dial.deleteSprite();          // delete Sprite
}

void drawDial()           // function to display clock face
{
  dial.createSprite(width, height);     // pivot point of clock face
  dial.setPivot(centX, centY);      //    on which clock hands rotate
  dial.setColorDepth(8);
  dial.fillScreen(TFT_DARKGREY);
  dial.drawRoundRect(0, 0, width, height, 10, TFT_WHITE); // screen border
  dial.fillCircle(centX, centY, dialR + 5, TFT_NAVY);      // blue outline circle
  dial.fillCircle(centX, centY, dialR, TFT_WHITE);    //    around clock
  lines (6, Lshort, TFT_BLACK);     // degrees between markers
  lines (30, Llong, TFT_BLUE);      // for one and five minutes
  dial.setTextColor(TFT_NAVY);
  dial.setTextSize(1);          // position day and month
  if(timeData.tm_min > 20 && timeData.tm_min < 40) {posx = 100; posy = 55;}
  else if(timeData.tm_min <= 20) {posx = 60; posy = 85;}
  else {posx = 140; posy = 85;}
  dial.drawString(day[wd], posx, posy, 4);    // display day of week
  dial.drawString(mon[mn], posx, posy+30, 4);   //   and month
  if(dd > 9) dial.drawNumber(dd, 90, 140, 7);   // date in fixed position
  else dial.drawNumber(dd, 110, 140, 7);

}

void lines (int increm, int len, int colour)  // function to draw minute
{               //    markers
  int x0, y0, x1, y1, val, dev;
  float sx, sy;
  for (int j=0; j<360; j=j+increm)      // increments of 6 or 30 degrees
  {
    sx = cos(j * DEG_TO_RAD);       // co-ordinates of marker
    sy = sin(j * DEG_TO_RAD);
    x0 = round(centX + dialR * sx);     // position on dial perimeter
    y0 = round(centY + dialR * sy);
    x1 = round(centX + (dialR - len) * sx); // position inside dial
    y1 = round(centY + (dialR - len) * sy);
    dial.drawLine(x0, y0, x1, y1, colour);  // draw minute marker
    if(increm == 30)          // for five minute markers
    {
      if(j > 270) val = (j-270)/30;     // 0 = right-side of dial
      else val = (j+90)/30;          // rotate values 90 anti-clockwise
      dev = 6;              // adjust for digits > 9
      if(val > 9) dev = 18;
      x1 = round(centX + (dialR + 15) * sx - dev);  // positioned at top-left
      y1 = round(centY + (dialR + 15) * sy - 10); //    corner of number
      dial.setTextColor(TFT_WHITE);
      dial.drawNumber(val, x1, y1, 4);      // numbers around dial
    }
  }
}

void drawHour(int wid, int len, int colour) // function to draw hour hand
{
  int wid2 = wid/2;
  hourh.setColorDepth(8);
  hourh.createSprite(wid, len);     // Sprite width and length
  hourh.fillSprite(TFT_WHITE);
  hourh.setPivot(wid2, 10);       // pivot point of hour hand
  hourh.fillRect(0, 0, wid, len, colour);
  hourh.drawLine(wid2, 0, wid2, len, TFT_BLACK);  // line through middle
}                 //    of rectangle

void drawMinute(int wid, int len, int colour) // function to draw minute hand
{
  int wid2 = wid/2;
  minuteh.setColorDepth(8);
  minuteh.createSprite(wid, len);
  minuteh.fillSprite(TFT_WHITE);
  minuteh.setPivot(wid2, 10);
  minuteh.fillRect(0, 0, wid, len, colour);
  minuteh.drawLine(wid2, 0, wid2, len, TFT_BLACK);
}

void drawSecond(int wid, int len, int colour) // function to draw second hand
{
  secondh.setColorDepth(8);
  secondh.createSprite(wid, len);
  secondh.fillSprite(TFT_WHITE);
  secondh.setPivot(1, 10);
  secondh.fillRect(0, 0, wid, len, colour);
}
