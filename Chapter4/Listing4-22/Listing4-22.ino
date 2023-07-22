#define LILYGO_WATCH_2020_V2
#include <LilyGoWatch.h>
#include <SD.h>       // include SD library
File file;          // associate file with SD library
SPIClass hspi(HSPI); // SD card using HSPI
TTGOClass * ttgo;
TFT_eSPI * tft;
AXP20X_Class * power;
TinyGPSPlus *gps;
int count = 0, SDcard = 0;
String text, filename = "/walk.txt";   // file name on SD card
String header = "count, lat, long, alt, total, speed, sat";
unsigned long lastTime = 0, lastBatt = 0;
double startLat, startLong, lastLat, lastLong, totalDist = 0;
int startFlag = 0;

void setup()
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->bl->adjust(48);
  ttgo->trunOnGPS();
  ttgo->gps_begin();
  gps = ttgo->gps;
  tft = ttgo->tft;
  power = ttgo->power;
  power->adc1Enable(AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, 1);
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE);
  tft->setCursor(0,0);
  tft->setTextSize(2);
  hspi.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if(!SD.begin(SD_CS, hspi)) tft->print("no SD card");  // check for presence of SD card
  else
  {
    SDcard = 1;
    tft->print("SD card OK");
    if(SD.exists(filename)) file = SD.open(filename, FILE_APPEND);
    else file = SD.open(filename, FILE_WRITE);
    file.print(header);
    file.println();
    file.close();
  }
  delay(1000);            // time for user to read message
}

void loop()
{
  ttgo->gpsHandler();          // handle GPS signals
  if (gps->location.isUpdated())      // change in GPS location
  {
    if(startFlag == 0)          // GPS updated at start position
    {
      startFlag = 1;
      startLat =  gps->location.lat();
      startLong = gps->location.lng();
      lastLat = startLat;
      lastLong = startLong;
    }
    if((millis() - lastTime > 15000) && startFlag == 1) // update GPS data
    {                   //    every 15s
      lastTime = millis();
      collectData();          // call function collect GPS data
      file = SD.open(filename, FILE_APPEND);  // open an existing file
      file.print(text);         // append string to file
      file.println();         //    and new line character
      file.close();
    }
  }
  if(millis() - lastBatt > 120000)      // battery voltage updated 2min
  {
    tft->setTextSize(2);
    tft->fillRect(0,216, 240,240, TFT_BLACK); // blank out previous value
    tft->setCursor(0,216);
    tft->printf("Battery %d  \n", power->getBattPercentage());
    lastBatt = millis();
  }
}

String collectData()          // function to collect GPS data
{
  double curLat, curLong, dist, startDist, alt;
  curLat = gps->location.lat();
  curLong = gps->location.lng();      // distance from previous position
  dist = TinyGPSPlus::distanceBetween(curLat, curLong, lastLat, lastLong);
  if(dist > 5) totalDist = totalDist + dist;  // increment total distance
  startDist = TinyGPSPlus::distanceBetween  // distance from start position
(curLat, curLong, startLat, startLong);
  lastLat = curLat;
  lastLong = curLong;
  alt = gps->altitude.meters();
  tft->setTextSize(3); 
  tft->fillRect(0,0, 240,215, TFT_BLACK);
  tft->setCursor(0,0);              // display values:
  tft->printf("Alt   %.1f \n", alt);        // altitude (m)
  tft->printf("\nDist  %.1f \n", dist);       // distance covered
  tft->print("Total ");tft->println(round(totalDist), 0); // total distance
  tft->print("Start ");tft->println(round(startDist), 0); //  and from start
  tft->printf("\nSpeed %.1f \n", gps->speed.kmph());    // speed (km/h)
  tft->printf("Satellites %d \n", gps->satellites.value());
  count++;                // current GPS position
  text = String(count)+","+String(curLat,6)+","+String(curLong,6)+",";
  text = text +String(alt)+","+String(totalDist)+","; // altitude and distance
  text = text +String(gps->speed.kmph())+",";    // speed (km/h)
  text = text +String(gps->satellites.value());   // number of satellites
  return text;
}
