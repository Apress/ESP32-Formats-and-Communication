/*******************************************************************************
 * Sketch name: Eddystone UID beacon
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

void setBeacon()          // function to generate beacon message
{
  char beaconData[20];            // beacon message array
  uint16_t UUID = 0xFEAA;         // Eddystone UUID
  BLEAdvertisementData advertData = BLEAdvertisementData();
  advertData.setCompleteServices(BLEUUID(UUID));
  beaconData[0] = 0x00;         // Eddystone frame type (UID)
  beaconData[1] = 0xF4;         // beacon TX power at 0m
  beaconData[2] = 0x05;         // UUID Namespace NID[0 to 9]
  beaconData[3] = 0x98;         // one byte or two characters
  beaconData[4] = 0x6D;         //     for each array element
  beaconData[5] = 0x33;
  beaconData[6] = 0xF9;         // 05986d33-f920-4802-bbad-
  beaconData[7] = 0x20;
  beaconData[8] = 0x48;
  beaconData[9] = 0x02;
  beaconData[10] = 0xBB;
  beaconData[11] = 0xAD;
  beaconData[12] = 0x0C;        // UUID Instance BID[0 to 5]
  beaconData[13] = 0xFA;        // 0cfa43d07079
  beaconData[14] = 0x43;
  beaconData[15] = 0xD0;
  beaconData[16] = 0x70;
  beaconData[17] = 0x79;
  beaconData[18] = 0x00;        // reserved to 0x00
  beaconData[19] = 0x00;
                // generate beacon message
  advertData.setServiceData(BLEUUID(UUID), std::string(beaconData, 20));
  advertise->setAdvertisementData(advertData);
  BLEAdvertisementData scanData = BLEAdvertisementData();
  scanData.setName("ESP32 UID Beacon");
  advertise->setScanResponseData(scanData);
}
