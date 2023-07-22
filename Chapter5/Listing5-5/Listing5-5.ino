/*******************************************************************************
 * Sketch name: iBeacon
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

void setBeacon()          // function to generate beacon message
{
  char UUID[] = "05986d33-f920-4802-bbad-0cfa43d07079";
  BLEBeacon beacon = BLEBeacon();
  beacon.setManufacturerId(0x4C00);     // manufacturer ID for Apple
  beacon.setProximityUUID(BLEUUID(UUID));
  beacon.setMajor(3 * reboot);      // 16-bit integer
  beacon.setMinor(reboot);        // 16-bit integer
  beacon.setSignalPower(0xBB);      // RSSI in two's complement
  BLEAdvertisementData advertData = BLEAdvertisementData();
  std::string serviceData = "";
  serviceData += (char)26;        // length of beacon message
  serviceData += (char)0xFF;        // iBeacon frame type
  serviceData += beacon.getData(); 
  advertData.addData(serviceData);      // generate beacon message
  advertise->setAdvertisementData(advertData);
  BLEAdvertisementData scanData = BLEAdvertisementData();
  scanData.setName("ESP32 iBeacon");
  advertise->setScanResponseData(scanData);
}
