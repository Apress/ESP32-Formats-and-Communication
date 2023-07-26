/*******************************************************************************
 * Sketch name: BLE scanner
 * Description: Determine names and RSSI of BLE devices
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

#include <BLEDevice.h>            // include BLE library
BLEScan * BLEScan;            // define scanner and
int scan = 5;             // scan time (secs)
int RSSI;
float dist;

class DeviceCallback : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {                 // BLE device name
    Serial.print("name ");Serial.print(advertisedDevice.getName().c_str());
    Serial.print(" address ");        // BLE device address
    Serial.print(advertisedDevice.getAddress().toString().c_str());
    RSSI = advertisedDevice.getRSSI();      // RSSI of received signal
    dist = pow(10, (-69.0-RSSI)/20.0);      // distance to BLE device
    Serial.printf(" RSSI %d distance %.2f \n", RSSI, dist);
  }
};
void setup()
{
  Serial.begin(115200);
  BLEDevice::init("");            // initialise BLE
  BLEScan = BLEDevice::getScan();       //     and BLE scanner
  BLEScan->setAdvertisedDeviceCallbacks(new DeviceCallback());
  BLEScan->setActiveScan(true);       // obtain a scan response
}

void loop()
{
  BLEScanResults res = BLEScan->start(scan, false);   // number of 
  Serial.printf("%d Devices found\n", res.getCount());  // scanned devices
  BLEScan->clearResults();
}
