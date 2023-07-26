/*******************************************************************************
 * Sketch name: Bluetooth device scanning
 * Description: Obtain Bluetooth device names and RSSI
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <BluetoothSerial.h>        // include library and associate
BluetoothSerial SerialBT;       //    SerialBT with library
int Ndev, lag = 5000;         // scan period of 5s

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  SerialBT.begin();         // initialise SerialBT
  Serial.printf("\nscanning for %ds \n", lag/1000);
  BTScanResults * BTdevs = SerialBT.discover(lag);  // scan for lag time
  Ndev = BTdevs->getCount();        // number of Bluetooth devices
  if(Ndev > 0)
  {
    for (int i=0; i<Ndev; i++)
    {               // display device name and RSSI
      BTAdvertisedDevice *device = BTdevs->getDevice(i);
      Serial.printf("%d. %s RSSI %d \n", i+1,
      device->getName().c_str(), device->getRSSI());
    }
  }
  else Serial.println("No BT device found");
}

void loop()
{}                // nothing in loop function
