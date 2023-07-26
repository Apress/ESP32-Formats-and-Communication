/*******************************************************************************
 * Sketch name: BLE transmission with GATT characteristics
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

#include <BLEDevice.h>          // include BLE library
#include <BLE2902.h>          //     and BLE2902 library
BLEServer * Server;         // define BLE server,
BLEService * battService;       //     service and service UUID
BLEUUID battServUUID = BLEUUID((uint16_t)0x180F);
BLECharacteristic battChar(BLEUUID((uint16_t)0x2A19), // define characteristic
                  BLECharacteristic::PROPERTY_NOTIFY);  // UUID
BLEDescriptor * battDesc;       // define BLE descriptor
BLE2902 * battNotfy;          // define notification status
BLEService * tempService;       // repeat for second service
BLEUUID tempServUUID = BLEUUID((uint16_t)0x181A);
BLECharacteristic tempChar(BLEUUID((uint16_t)0x2A6E),
                  BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor * tempDesc;
BLE2902 * tempNotfy;
unsigned long lag;
int batt = 1, clientCon = 0;
float temp;
uint8_t tempData[2];

class ServerCallback : public BLEServerCallbacks  // callback function
{
  void onConnect(BLEServer * Server)      // client connect to server
  {
    clientCon = 1;
    Serial.println("connected to client");
    delay(500);
  }
  void onDisconnect(BLEServer * Server)   // client disconnect from server
  {
    clientCon = 0;
    delay(500);
    Server->startAdvertising();     // server start advertising
    Serial.println("client disconnected\nstarted advertising");
  }
};

void setup()
{
  Serial.begin(115200);
  BLEDevice::init("ESP32");       // define BLE server name
  Server = BLEDevice::createServer();
  Server->setCallbacks(new ServerCallback());
  battService = Server->createService(battServUUID);  // add service to server
  battService->addCharacteristic(&battChar);  // add characteristic to service
  battDesc = new BLE2902();
  battChar.addDescriptor(battDesc);     // add descriptor to characteristic
  battNotfy = (BLE2902*)battDesc;     // link notifier to descriptor
  battService->start();
  tempService = Server->createService(tempServUUID); // repeat for second service
  tempService->addCharacteristic(&tempChar);
  tempDesc = new BLE2902();
  tempChar.addDescriptor(tempDesc);
  tempNotfy = (BLE2902*)tempDesc;
  tempService->start();
  lag = 0;
  Server->getAdvertising()->start();    // start advertising services
}

void loop()
{
  if(clientCon == 1  && millis() – lag > 5000)  // client connected to server
  {               // and advertising period expired
    batt++;
    if(batt > 99) batt = 1;
    battChar.setValue(batt);
    battChar.notify();          // notify battery service update
    temp = batt*1.11;
    tempData[0] = (uint16_t)(temp*100);
    tempData[1] = (uint16_t)(temp*100) >> 8;  // upper and lower bytes
    tempChar.setValue(tempData, 2);     // temperature format uint16_t
    tempChar.notify();
    Serial.printf("notify %d batt %d notify %d temp %.2f \n", 
                   battNotfy->getNotifications(), batt,
                   tempNotfy->getNotifications(), temp);
    lag = millis();
  }
}
