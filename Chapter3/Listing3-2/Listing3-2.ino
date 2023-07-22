/*******************************************************************************
 * Sketch name: MESH module with LED and generated data (node 3)
 * Description: Receiving node with callback
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    3 - MESH communication
 ******************************************************************************/

#include <painlessMesh.h>        // differences from Listing 3-1
painlessMesh mesh;
Scheduler scheduler;
String ssid = "meshSSID";
String password = "meshPass";
int port = 5555;
int LEDpin = 2;             // define LED pin
int LEDstate, oldLEDstate;          // new and old LED states
DynamicJsonDocument doc(512);

void sendMessage();
Task taskSend(TASK_SECOND * 1 , TASK_FOREVER, &sendMessage);

void setup()
{
  pinMode(LEDpin, OUTPUT);          // LED pin as OUTPUT
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
}

void sendMessage()          // function to send message
{
  float value = random(0, 100);     // generate random number
  doc["value"] = value;           // update name-value pair
  String sendMsg;
  serializeJson(doc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  taskSend.setInterval((TASK_SECOND * 3));    // message timing of 3s
}

void recvMessage(uint32_t sender, String &recvMsg)
{
  deserializeJson(doc, recvMsg);
  LEDstate = doc["LED"];          // update LED state
  digitalWrite(LEDpin, LEDstate);       //      turn on or off LED
  if(LEDstate != oldLEDstate)
  {                 // if LED state changed
    oldLEDstate = LEDstate;         //      update old LED state
    mesh.sendSingle(sender, recvMsg);     // send message to node 1
  }
}

void loop()
{
  mesh.update();
}
