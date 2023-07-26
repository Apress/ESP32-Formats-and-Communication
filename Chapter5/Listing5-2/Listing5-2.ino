/*******************************************************************************
 * Sketch name: Eddystone URL beacon
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

#include <BLEDevice.h>          // include BLEDevice library
#include <sys/time.h>         // and the system time library
int sleepSec = 5;           // sleep 5 seconds, then wake up
unsigned long lag, uSec = 1000000;
RTC_DATA_ATTR int reboot = 0;       // number of reboots and last
RTC_DATA_ATTR time_t lastReboot;      // reboot time in RTC memory
struct timeval timeData;        // time (s) since reset
BLEAdvertising *advertise;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  gettimeofday(&timeData, NULL);      // time since reset
  Serial.printf("%d reboots ", reboot++);   // number and timing of reboots
  Serial.printf("%ds since reboot \n", timeData.tv_sec - lastReboot);
  lastReboot = timeData.tv_sec;     // time(s) of last reboot
  BLEDevice::init("");
  advertise = BLEDevice::getAdvertising();
  setBeacon();            // call beacon function
  lag = 0;              // reset time lag
  advertise->start();         // start beacon advertising
  Serial.println("start advertising");
}

void setBeacon()          // function to generate beacon message
{
  String URL = "bit.ly/3y0iW6V";      // URL to be broadcast
  char beaconData[17];          // beacon message array
  uint16_t UUID = 0xFEAA;       // Eddystone UUID
  BLEAdvertisementData advertData = BLEAdvertisementData();
  advertData.setCompleteServices(BLEUUID(UUID));
  beaconData[0] = 0x10;         // Eddystone frame type (URL)
  beaconData[1] = 0xF4;         // beacon TX power at 0m = -12
  beaconData[2] = 0x03;         // URL scheme for https://
  for (int i=0; i<URL.length(); i++) beaconData[i+3] = URL[i];
                // generate beacon message
  advertData.setServiceData(BLEUUID(UUID), std::string(beaconData, 17));
  advertise->setAdvertisementData(advertData);
  BLEAdvertisementData scanData = BLEAdvertisementData();
  scanData.setName("ESP32 URL Beacon");   // define beacon name
  advertise->setScanResponseData(scanData);
}

void loop()
{
  if(millis() – lag > 5000)
  {
    advertise->stop();          // stop beacon advertising
    Serial.println("stop advertising");
    esp_deep_sleep(sleepSec * uSec);    // sleep for sleepSec seconds
  }
}
