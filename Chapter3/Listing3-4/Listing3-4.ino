/*******************************************************************************
 * Sketch name: ESP-MESH and Bluetooth communication (node 3)
 * Description: Receiving node with callback
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    3 - MESH communication
 ******************************************************************************/

#include <painlessMesh.h>
painlessMesh mesh;
Scheduler scheduler;
String ssid = "meshSSID";
String password = "meshPass";
int port = 5555;
int LEDpin = 2;
int LEDstate, oldLEDstate;
#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
String sendBT, oldBT = "";
DynamicJsonDocument doc(512);

void sendMessage();
Task taskSend(TASK_SECOND * 1 , TASK_FOREVER, &sendMessage);

void setup()
{
  pinMode(LEDpin, OUTPUT);        // LED pin as OUTPUT
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
  SerialBT.begin("ESP32 right");      // identify Bluetooth device
}

void sendMessage()
{
  float value = random(0, 100);
  doc["value"] = value;         // update name-value pair
  doc["text"] = sendBT;
  String sendMsg;
  serializeJson(doc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  taskSend.setInterval((TASK_SECOND * 3));
  sendBT = "";
}

void recvMessage(uint32_t sender, String &recvMsg)
{
  deserializeJson(doc, recvMsg);
  String recvBT = doc["text"];       // received text
  if(recvBT != oldBT)
  {               // transmit to receiving
    if(recvBT.length() > 3) SerialBT.print(recvBT); //     Android tablet
    oldBT = recvBT;
  }
  LEDstate = doc["LED"];        // update LED state
  digitalWrite(LEDpin, LEDstate);     // turn on or off LED
  if(LEDstate != oldLEDstate)
  {
    oldLEDstate = LEDstate;
    mesh.sendSingle(sender, recvMsg);   // transmit new LED state
  }               //     to node 1
}

void loop()
{
  mesh.update();
  if(SerialBT.available())        // new Bluetooth message
    sendBT = SerialBT.readString();     // read Bluetooth Serial buffer
}
