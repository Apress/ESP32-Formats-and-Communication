/*******************************************************************************
 * Sketch name: WS2812 colour selection with Wi-Fi
 * Description: Alternative to Bluetooth in Listing 12-5
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    12 - Control apps
 ******************************************************************************/

#include <WiFi.h>            // include WiFi library
WiFiClient client;          // associate client and server with
WiFiServer server(80);        //     WiFiClient and WiFiServer libraries
#include <ssid_password.h>        // file with SSID and password
#include <Adafruit_NeoPixel.h>      // include NeoPixel library
int LEDpin = 25;            // define data pin
int LEDnumber = 12;         // number of LEDs on ring
int32_t color;            // color is 32-bit or signed long
              // associate ring with NeoPixel library
Adafruit_NeoPixel ring(LEDnumber, LEDpin, NEO_GRB + NEO_KHZ800);
String str;
int R,G,B, indexS, indexF;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display server IP address
  server.begin();           // initiate server
}

void loop()
{
  client = server.available();
  if (client)           // initialise client connection
  {
    while (!client.available()) {};     // no client request, do nothing
    str = client.readStringUntil('\r');   // map str to message
    Serial.println(str);        // display HTTP GET request
    indexS = str.indexOf("R=");     // position of "R=" in string
    indexF = str.indexOf("G=");
    R = str.substring(indexS, indexF-1);    // red colour component
    indexS = indexF;
    indexF = str.indexOf("B=");
    G = str.substring(indexS, indexF-1);    // green colour component
    indexS = indexF;
    indexF = str.indexOf("H");
    B = str.substring(indexS, indexF-1);    // blue colour component
    Serial.printf("RGB: %d %d %d \n", R, G, B); // display colour components
    color = ring.Color(R, G, B);      // convert RGB values to colour
    ring.fill(color);         // set the LED colour
    ring.show();            // update LED ring colour
    client.stop();          // close connection
  }
}
