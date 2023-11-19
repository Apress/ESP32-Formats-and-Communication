/*******************************************************************************
 * Sketch name: Eddystone TLM beacon
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

void setBeacon()          // function to generate beacon message
{
  char beaconData[14];            // beacon message array
  uint16_t UUID = 0xFEAA;         // Eddystone UUID
  uint16_t voltage = reboot * 12;       // generated voltage
  float temp = (reboot * 1.1);        //  and temperature
  uint16_t tempInt = (int)(temp * 256.0 + 1.0);   // 8.8 fixed point notation
  BLEAdvertisementData advertData = BLEAdvertisementData();
  advertData.setCompleteServices(BLEUUID(UUID));
  beaconData[0] = 0x20;           // frame type (TLM)
  beaconData[1] = 0x00;           // TLM version
  beaconData[2] = (voltage >> 8);       // stored as 16-bit number
  beaconData[3] = (voltage & 0xFF);
  beaconData[4] = (tempInt >> 8);       // 8.8 fixed point notation
  beaconData[5] = (tempInt & 0xFF);
  beaconData[6] =  (reboot >> 24);        // stored as 32-bit number
  beaconData[7] = ((reboot >> 16) & 0xFF);
  beaconData[8] = ((reboot >> 8)& 0xFF);
  beaconData[9] =  (reboot & 0xFF);
  beaconData[10] =  (timeData.tv_sec*10 >> 24);    // stored as 32-bit number
  beaconData[11] = ((timeData.tv_sec*10 >> 16) & 0xFF);  // with 0.1sec resolution
  beaconData[12] = ((timeData.tv_sec*10 >> 8) & 0xFF);
  beaconData[13] =  (timeData.tv_sec*10 & 0xFF);      // generate beacon message
  advertData.setServiceData(BLEUUID(UUID), std::string(beaconData, 14));
  advertise->setAdvertisementData(advertData);
  BLEAdvertisementData scanData = BLEAdvertisementData();
  scanData.setName("ESP32 TLM Beacon");
  advertise->setScanResponseData(scanData);
}
