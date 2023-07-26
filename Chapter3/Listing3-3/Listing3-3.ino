/*******************************************************************************
 * Sketch name: ESP-MESH and Bluetooth communication (node 1)
 * Description: Transmitting (control) node
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
int LEDstate = 0, relayState = 0;      // LED and relay states
#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
String sendBT, oldBT = "";        // Bluetooth messages
char c;             // character for command letter
DynamicJsonDocument doc(512);

void sendMessage();
Task taskSend(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

void setup()
{
  Serial.begin(115200);
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
  SerialBT.begin("ESP32 left");     // identify Bluetooth device
}

void sendMessage()
{
  doc["relay"] = relayState;
  doc["LED"] = LEDstate;
  doc["text"] = sendBT;          // text for Android tablet
  String sendMsg;
  serializeJson(doc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  taskSend.setInterval((TASK_SECOND * 1));
  sendBT = "";            // reset Bluetooth text message
}

void recvMessage(uint32_t sender, String &recvMsg)
{
//  Serial.printf("from %u %s \n", sender, recvMsg.c_str());  
  deserializeJson(doc, recvMsg);
  String recvBT = doc["text"];       // text from Android tablet
  Serial.printf("value %d \n", doc["value"].as<long>());
  if(recvBT != oldBT)
  {                 // transmit to receiving
    if(recvBT.length() > 3) SerialBT.print(recvBT); //     Android tablet
    oldBT = recvBT;
  }
}

void loop()
{
  mesh.update();
  if(SerialBT.available())          // new Bluetooth message
  {
    sendBT = SerialBT.readString();       // read Serial buffer
    c = sendBT[0];            // first letter of message
    switch (c)              // switch...case on letter
    {
    case 'L':             // letter = L (for LED)
      LEDstate = 1-LEDstate;          // alternate the LED state
      if(LEDstate == 1) SerialBT.println("LED on"); // transmit LED state to
      else SerialBT.println("LED off");     //      Android tablet
      break;
    case 'R':
      relayState = 1-relayState;        // similarly for the relay
      if(relayState == 1) SerialBT.println("relay on");
      else SerialBT.println("relay off");
      break;
    default: break;           // no action, not L nor R
    }
  }
}
