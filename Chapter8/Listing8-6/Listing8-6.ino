/*******************************************************************************
 * Sketch name: Over The Air
 * Description: Update sketch with OTA to remote ESP32
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <ElegantOTA.h>          // include ElegantOTA library
WebServer server(80);         // initialise server
#include <ssid_password.h>
char OTAhost[] = "ESP32board";      // OTA name and password
char OTApass[] = "admin1";
int LEDpin = 2;
int lag = 1000;            // LED lag time (ms)
String str;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);
  WiFi.mode(WIFI_STA);          // Wi-Fi station mode
  WiFi.begin(ssid, password);      // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address: ");     // display ESP32 IP address
  Serial.println(WiFi.localIP());
  ElegantOTA.begin(&server);        // start ElegantOTA
  server.begin();           // start server
  server.on("/", base);         // load default webpage
}

void loop()
{
  server.handleClient();        // manage HTTP requests
  digitalWrite(LEDpin, !digitalRead(LEDpin)); // turn on or off LED
  delay(lag);
}

void base()           // function called when webpage loaded
{
  str = "Time lag " + String(lag);
  server.send(200, "text/plain", str);    // send response to client
}
