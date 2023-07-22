/*******************************************************************************
 * Sketch name: BLE with Serial communication for client
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

#include <BLEDevice.h>          // include BLE library
BLEAddress * ServerAddress;       // define server address,
BLEScan * BLEScan;          //     scanner,
BLEClient * Client;         //     client
BLERemoteService * Service;       //     service
BLERemoteCharacteristic * Char;     //     characteristic
BLERemoteDescriptor * Desc;       //     descriptor
#include <TFT_eSPI.h>         // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();
char ServerName[] = "ESP32";        // name of required server
char ServUUID[] = "6e400001-b5a3-f393-e0a9-e50e24dcca9e"; // Service UUID
char CharUUID[] = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"; // receive UUID
char * msg;
float dist;
int RSSI, clientCon = 0, serverFnd = 0;
int leftButton = 0, rightButton = 35;   // module buttons

class DeviceCallback : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {               // scanned servers
    if (advertisedDevice.getName() == ServerName) // check server name
    {           // stop scanning when required server found
      advertisedDevice.getScan()->stop();
      RSSI = advertisedDevice.getRSSI();    // RSSI of received signal
      dist = pow(10, (-69.0-RSSI)/20.0);    // distance to server
      tft.fillScreen(TFT_BLUE);
      tft.setCursor(0,0);
      tft.printf("RSSI %d dist %.2f \n", RSSI, dist);
      ServerAddress = new BLEAddress(advertisedDevice.getAddress());
      serverFnd = 1;          // found required server
    }
  }
};

class ClientCallback : public BLEClientCallbacks  // callback function
{
  void onConnect(BLEClient* client)     // client connect to server
  {
    clientCon = 1;
    tft.printf("connected to %s \n", ServerName); // display server name
    delay(500);
  }
  void onDisconnect(BLEClient* client)
  {
    serverFnd = 0;
    clientCon = 0;
    tft.fillScreen(TFT_BLUE);         // clear screen
    tft.setCursor(0,20);
    tft.printf("%s disconnect \n", ServerName);
  }
};

void setup()
{
  pinMode(leftButton, INPUT_PULLUP);    // buttons active LOW
  pinMode(rightButton, INPUT_PULLUP);
  tft.init();
  tft.setRotation(3);         // landscape USB to left
  tft.fillScreen(TFT_BLUE);       // blue screen for Bluetooth
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(0,0);
  tft.println("BLE Client");
  BLEDevice::init("");          // initialise client
  BLEScan = BLEDevice::getScan();     //     and BLE scanner
            // callback to notify BLE device detected
  BLEScan->setAdvertisedDeviceCallbacks(new DeviceCallback());
  BLEScan->setActiveScan(true);     // active obtains a scan response
}

void serverConnect(BLEAddress Address)      // connect to BLE server
{
  tft.printf("%s \n", Address.toString().c_str());  // server MAC addres
  Client = BLEDevice::createClient();     // initialise BLE client
            // callback to manage connection to server
  Client->setClientCallbacks(new ClientCallback());
  Client->connect(Address);         // connect to server
  Service = Client->getService(ServUUID);     // initialise BLE service
  Char = Service->getCharacteristic(CharUUID);    //     add characteristic
  Desc = Char->getDescriptor(BLEUUID((uint16_t)0x2902));  // add descriptor
  Char->registerForNotify(callback);      // request notifications
}

void callback(BLERemoteCharacteristic * BLERemoteChar,
     uint8_t * data, size_t length, bool notfy)
{               //callback to manage notification
  msg = (char*)data;          // convert to character array
  tft.setCursor(0,80);
  tft.fillRect(0,80, 240, 50, TFT_BLUE);    // clear previous notification
  tft.printf("message len %d \n", length);  // update displayed notification
  for (int i=0; i<length; i++) tft.print(msg[i]);
}

void notify(String state)       // function to request notifications
{
  uint8_t data[] = {0x01, 0};       // define notification status as on
  if(state == "off") data[0] = 0x0;     //     or off
  Desc->writeValue((uint8_t*)data, 2, true);  // transmit status to server
  tft.setCursor(0, 50);
  tft.printf("notify %s \n", state);    // update displayed status
}

void loop()
{                // connect to server
  if(serverFnd == 1 && clientCon == 0) serverConnect(*ServerAddress);
                // otherwise scan 5s for server
  else if(serverFnd == 0) BLEScan->start(5, false);
              // buttons to turn on or off notifications
  if (digitalRead(leftButton) == LOW && clientCon == 1) notify("off");
  else
  if (digitalRead(rightButton) == LOW && clientCon == 1) notify("on ");
  delay(100);           // crude debounce of buttons
}
