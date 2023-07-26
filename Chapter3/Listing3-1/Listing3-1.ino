/*******************************************************************************
 * Sketch name: MESH module with buttons and display (node 1)
 * Description: Transmitting (control) node
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    3 - MESH communication
 ******************************************************************************/

#include <painlessMesh.h>        // include library
painlessMesh mesh;          // associate mesh with library
Scheduler scheduler;          // associate scheduler with library
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
String ssid = "meshSSID";       // ESP-MESH name & password
String password = "meshPass";
int port = 5555;            // ESP-MESH port
int relayButton = 0, LEDbutton = 35;    // LED and relay buttons
int LEDstate, relayState;
DynamicJsonDocument doc(512);       // JSON document

void sendMessage();
Task taskSend(TASK_SECOND * 1, TASK_FOREVER, &sendMessage); // task timing of 1s

void setup()
{
  Serial.begin(115200);
  pinMode(relayButton, INPUT_PULLUP);     // buttons active LOW
  pinMode(LEDbutton, INPUT_PULLUP);
  mesh.setDebugMsgTypes(ERROR | STARTUP);     // before init instruction
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);          // set recvMessage function
  mesh.onNewConnection(&newConnectionCallback);   // ESP-MESH functions
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  scheduler.addTask(taskSend);        // schedule and enable
  taskSend.enable();            // send message function
  tft.init();             // initialise screen
  tft.setRotation(3);           // landscape USB on left
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);          // clear screen
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0,0);
  tft.print("Relay");           // label buttons
  tft.setCursor(0,110);
  tft.print("LED");
  tft.setCursor(0,30);
  tft.print("nodes 0");
}

void sendMessage()          // function to send message
{                 // change relay state
  if (digitalRead(relayButton) == LOW) relayState = 1-relayState;
  doc["relay"] = relayState;          // update JSON document
  if (digitalRead(LEDbutton) == LOW) LEDstate = 1-LEDstate;
  doc["LED"] = LEDstate;
  String sendMsg;
  serializeJson(doc, sendMsg);        // serialise JSON doc
  mesh.sendBroadcast(sendMsg);        // broadcast message
  taskSend.setInterval((TASK_SECOND * 1));    // message timing of 1s
}
                // function to receive message
void recvMessage(uint32_t sender, String &recvMsg)
{
  Serial.printf("from %u %s \n", sender, recvMsg.c_str());         // sender identity and message
  DeserializationError error = deserializeJson(doc, recvMsg);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  tft.setCursor(0,0);
  if(doc["relay"].as<long>() == 1) tft.print("Relay ON ");  // relay state
  else tft.print("Relay OFF");
  tft.setCursor(0,60);              // generated data
  tft.printf("value %d   \n", doc["value"].as<long>());
  tft.setCursor(0,110);
  if(doc["LED"].as<long>() == 1) tft.print("LED ON ");  // LED state
  else tft.print("LED OFF");
}

void newConnectionCallback(uint32_t nodeID)   // new node connected
{                 // display node identity
  Serial.printf("New connection nodeID %u \n", nodeID);
}

void changedConnectionCallback()      // function to list connected nodes
{
  SimpleList<uint32_t> list = mesh.getNodeList();   // number of nodes
  Serial.printf("number of nodes %d \n", list.size());
  tft.setCursor(0,30);
  tft.printf("nodes %d \n", list.size());
  Serial.println("Connection list");
  Serial.printf("node1 %u \n", mesh.getNodeId());   // node 1 identity
  SimpleList<uint32_t>::iterator node = list.begin();
  while (node != list.end())            // list of nodes
  {
    Serial.printf("nodeID %u \n", *node);       // *node is pointer
    node++;                 // increment node
  }
}

void nodeTimeAdjustedCallback(int32_t offset) // display synchronised timing
{
  Serial.printf("adjust %u offset = %d \n", mesh.getNodeTime(), offset);
}

void loop()
{
  mesh.update();              // handle ESP-MESH
}
