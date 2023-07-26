void setBeacon()          // function to generate beacon message
{
  String text;              // message up to 20 char.
  if((reboot % 2) == 1) text = "abcdefghijklmnopqrst";
                   else text = "ghijklmnopqrstuvwxyz";
  char beaconData[20];            // beacon message array
  uint16_t UUID = 0xFEAA;         // Eddystone UUID
  BLEAdvertisementData advertData = BLEAdvertisementData();
  advertData.setCompleteServices(BLEUUID(UUID));
  for (int i=0; i<text.length(); i++) beaconData[i] = text[i];
  advertData.setServiceData(BLEUUID(UUID), std::string(beaconData, 20));
  advertise->setAdvertisementData(advertData);
}
