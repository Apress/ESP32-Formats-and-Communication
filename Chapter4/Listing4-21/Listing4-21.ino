#define LILYGO_WATCH_2020_V2
#include <LilyGoWatch.h>
#include <SD.h>            // include SD library
File file;              // associate file with SD library
SPIClass hspi(HSPI);          // SD card uses HSPI
TTGOClass * ttgo;           // associate objects with libraries
TFT_eSPI * tft;
AXP20X_Class * power;         // required to access temperature
int count = 0, SDcard = 0;
String text, filename = "/temp.txt";    // file name on SD card
String header = "count, bright, temp, discharge";   // data header
int levels[] = {8, 16, 32, 64, 128, 192, 255};      // brightness levels

void setup()
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();            // initialise ttgo object
  ttgo->openBL();           // turn on backlight
  tft = ttgo->tft;          // shorthand for library objects
  power = ttgo->power;
  power->adc1Enable(AXP202_BATT_CUR_ADC1, true);  // ADC for battery current
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE);
  tft->setCursor(0,0);
  tft->setTextSize(2);
  hspi.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);   // connect micro-SD card
  if(!SD.begin(SD_CS, hspi)) tft->print("no SD card");
  else              // check for presence of SD card
  {
    SDcard = 1;           // micro-SD card detected
    tft->print("SD card OK");
    file = SD.open(filename, FILE_WRITE);   // open new file
    file.print(header);         // write column header to SD card
    file.println();         //    and new line character
    file.close();           // close file after writing
  }
  delay(1000);            // time for user to read message
}

void loop()
{
  for (int i=0; i<7; i++)       // different brightness levels
  {
    ttgo->setBrightness(levels[i]);     // change brightness
    for (int j=0; j<100; j++)       // 100 measurements per level
    {
      collectData(levels[i]);
      if(SDcard == 1)
      {
        file = SD.open(filename, FILE_APPEND);
        file.print(text);       // append string to file
        file.println();         //    and new line character
        file.close();
      }
    }
    delay(10000);           // 10s interval between levels
  }
  tft->fillScreen(TFT_BLACK);
  tft->setCursor(0, 40);
  tft->printf("data complete");
  while(1) {}           // do nothing
}

String collectData(int level)
{
  float temp, chrg_n;
  temp = power->getTemp();        // microcontroller temperature
  chrg_n = power->getBattDischargeCurrent();  // battery discharge current
  count++;
  tft->fillScreen(TFT_BLACK);
  tft->setCursor(40, 40);
  tft->printf("Bright %d \n", level); 
  tft->setCursor(40, 70);
  tft->printf("Temp   %.1f \n", temp);    // display values
  tft->setCursor(40, 100);
  tft->printf("chrg-  %.0fmA \n", chrg_n);
  tft->setCursor(40, 130);
  tft->printf("count  %d \n", count);
  text = String(count)+","+String(level);   // convert data to a string
  text = text +","+String(temp,1) +","+String(chrg_n,0);
  return text;
}
