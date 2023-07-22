/*******************************************************************************
 * Sketch name: app to control servo motor
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

#include <WiFi.h>            // include WiFi library
WiFiClient client;          // associate client and server with
WiFiServer server(80);        //     WiFiClient and WiFiServer libraries
char ssidAP[] = "ESP32";        // softAP SSID and password
char passwordAP[] = "12345678";
//#include <ssid_password.h>
String str, NNN;
int indexS, indexF;
int servoPin = 16;          // servo motor pin
int channel = 1, freq = 50, resol = 8;  // ledc channel, frequency and resolution
int low = 350, high = 2150;       // pulses to move to 0 and 180
int angle, pulse, duty;
float scalar;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.softAP(ssidAP, passwordAP);      // connect to softAP
  Serial.println(WiFi.softAPIP());      // display softAP IP address
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) delay(500);
//  Serial.println(WiFi.localIP());
  server.begin();           // initiate server
  ledcAttachPin(servoPin, channel);     // initialise ledc channel
  ledcSetup(channel, freq, resol);
  scalar = freq*(pow(2,resol)-1)/pow(10,6);
}

void loop()
{
  client = server.available();
  if (client)           // initialise client connection
  {
    while (!client.available()) {};     // no client request, do nothing
    str = client.readStringUntil('\r');   // map str to message
    Serial.println(str);
    indexS = str.indexOf("value");      // position of "value" in string
    indexF = str.indexOf("HTTP");
    NNN = str.substring(indexS+6, indexF-1);  // value=NNN HTTP
    client.println("HTTP/1.1 200 OK");    // HTTP header & response code
    client.println("Content-type:text/html\n"); // \n to generate blank line
    client.stop();          // close connection
    angle = NNN.toInt();        // servo motor angle
    pulse = low + (high-low)*angle/180;   // square wave pulse length
    duty = round(pulse*scalar);     // duty cycle
    ledcWrite(channel, duty);       // generate square wave
  }
}
