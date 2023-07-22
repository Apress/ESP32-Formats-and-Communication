/*******************************************************************************
 * Sketch name: Main sketch hosting application sketches
 * Description: Watch functions and applications
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

#include "config.h"          // incorporate libraries
TTGOClass * ttgo;
TFT_eSPI * tft;
AXP20X_Class * power;         // associate objects with libraries
BMA * bma;
Adafruit_DRV2605 * drv;
TinyGPSPlus * gps;
BluetoothSerial SerialBT;
IRsend irsend(TWATCH_2020_IR_PIN);      // references GPIO 2
unsigned long lastTime = 0;       // time display last updated
int IRQ = false;            // power button interrupt
int irq = false;            // step counter interrupt
uint8_t hh, mm, ss;         // variables for displayTime app
int bright = 3;           //      and Bright app

void setup()
{
  
  ttgo = TTGOClass::getWatch();
  ttgo->begin();            // initialise ttgo object
  ttgo->openBL();           // turn on backlight
  ttgo->rtc->check();         // compare RTC to compile time
  ttgo->rtc->syncToSystem();        // synchronise to system time
  ttgo->trunOnGPS();          // trunOn is NOT a typo
  ttgo->gps_begin();          // initialise GPS
  ttgo->enableDrv2650();        // enable DRV2605 motor driver
  tft = ttgo->tft;
  power = ttgo->power;          // shorthand for library objects
  bma = ttgo->bma;
  drv = ttgo->drv;
  gps = ttgo->gps;
  power->adc1Enable(AXP202_VBUS_VOL_ADC1 | // initialise battery ADC
      AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, 1);
  power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, 1); // enable button interrupt
  power->clearIRQ();            // reset interrupt
  pinMode(AXP202_INT, INPUT_PULLUP);    // power button interrupt
  attachInterrupt(AXP202_INT, [] {IRQ = 1;}, FALLING);
  pinMode(BMA423_INT1, INPUT);      // accelerometer interrupt
  attachInterrupt(BMA423_INT1, [] {irq = 1;}, RISING);
  ttgo->bl->adjust(64);         // adjust brightness from 0 to 255
  displayTime(1, bright);       // display default screen
}

void loop()
{
  if(millis() - lastTime > 1000)      // call displayTime function
  {               //      every second
    lastTime = millis();        // with full display updated
    displayTime(ss == 0, bright);     //      every minute
  }
  int16_t x, y;
  if (ttgo->getTouch(x, y))       // watch screen touched
  {
    while (ttgo->getTouch(x, y)) {}     // wait for touch release
    switch (menu())         // switch-case based on menu
    {               //      function output
      case 0: break;          // zero displays default screen
      case 1: bright = appBright(); break;  // change screen brightness
      case 2: appGPS(); break;      // position data from GPS
      case 3: appBattery(); break;      // display battery status
      case 4: appBlue(); break;     // Bluetooth communication
      case 5: appSonyIR(); break;     // transmit infrared signal
      case 6: appStep(); break;     // step counter
      case 7: appTemp(); break;     // microcontroller temperature
      case 8: appTimer(); break;     // count-down and count-up timer
      case 9: appWeather(); break;     // display OpenWeatherMap data
      case 10: appWwwTime(); break;     // synchronise with NTP time
      case 11: appGPSout(); break; // data from GPS
      case 12: appSatellite(); break; // GPS satellite positions
    }
    displayTime(1, bright);       // display default screen
  }
  if (IRQ) shutdown();          // power button pressed
}               //     to call shutdown function

void shutdown()           // AXP202 interrupt triggered
{
  power->clearIRQ();          // clear interrupt status
  ttgo->displaySleep();         // screen and touch in sleep mode
  power->setPowerOutPut(AXP202_LDO2, 0);    // disable backlight
  power->setPowerOutPut(AXP202_LDO3, 0);    // disable touch screen function
  power->setPowerOutPut(AXP202_LDO4, 0);    // disable GPS
  power->setPowerOutPut(AXP202_EXTEN, 0);   // disable Touch Reset Enable
  power->setPowerOutPut(AXP202_DCDC2, 0);   // wake up on power button
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_deep_sleep_start();       // initiate sleep mode
}
