/*******************************************************************************
 * Sketch name: Control DC motors with heading angle
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

#include <WiFi.h>            // include WiFi library
WiFiClient client;          // associate client and server
WiFiServer server(80);          //     with library
#include <ssid_password.h>        // file with SSID and password
int pins[] = {12, 13, 15, 14};      // DC motor pins
int chans[] = {1, 2, 3, 4};       // ledc channels
int freq = 1000, resol = 8;       //     frequency and resolution
int minDuty = 50, maxDuty;        // min and max scaled duty cycles
float scalar = 2.0, buffer = 0.1;
float sinDeg, cosDeg, FB, LR, Sum, Dif;
int degree;
int motorR, motorL, chanR, chanRnull, chanL, chanLnull;
String str, NNN;
int indexS, indexF;
float a, b;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display server IP address
  server.begin();           // initiate server
  for (int i=0; i<4; i++)
  {
    ledcAttachPin(pins[i], chans[i]);   // match pins[] to channels
    ledcSetup(chans[i], freq, resol);   //    and define PWM square wave
  }
}

void loop()
{
  client = server.available();      // initialise client connection
  if(client)
  {
    while (!client.available()) {};     // no client request, do nothing
    str = client.readStringUntil('\r');   // map str to HTTP GET request
    indexS = str.indexOf("heading");    // position of "heading" in string
    indexF = str.indexOf("&max");
    NNN = str.substring(indexS+8, indexF);  // heading=NNN &max
    degree = NNN.toInt();       // transform received value
    indexS = str.indexOf("maxSpd");
    indexF = str.indexOf("HTTP");
    NNN = str.substring(indexS+7, indexF-1);  // maxSpd=NNN HTTP
    maxDuty = NNN.toInt();
    client.println("HTTP/1.1 200 OK");    // HTTP header & response code
    client.println("Content-type:text/html\n"); // \n to generate blank line
    client.stop();          // close connection
    if(degree == 999) motor(0,0,0,0);   // stop DC motors
    else convert();         // call function to convert
  }               //     degree to DC motor speeds
}

void convert()        // function to convert degree to DC motor speeds
{
    sinDeg = sin(DEG_TO_RAD*degree);      // point co-ordinates from
    cosDeg = cos(DEG_TO_RAD*degree);      //    heading angle
    FB = 0;
    LR = 0;
    if(abs(sinDeg) > buffer)        // forward-backward component
       FB = minDuty + (maxDuty - minDuty) * abs(sinDeg);
    if(abs(cosDeg) > buffer)        // left-right component
       LR = minDuty + (maxDuty - minDuty) * abs(cosDeg);
    Sum = FB + LR/scalar;       // sum and difference of
    Dif = FB - LR/scalar;       //     FB and LR components
    if(Sum > maxDuty) Sum = maxDuty;    // constrain values
//  adjust();
//  Sum = a + Sum * b;
    if(Dif < minDuty) Dif = 0;
    motorL = 0;
    motorR = 0;           // map motor speed to each motor
    if(cosDeg < 0) {motorR = round(Sum); motorL = round(Dif);}
    else           {motorR = round(Dif); motorL = round(Sum);}
    if(sinDeg < 0) motor(0, motorL, 0,  motorR);    // move backward
    else           motor(motorL, 0,  motorR, 0);    // move forward
}
          // function to create PWM signal on each channel
void motor(int leftF, int leftB, int rightF, int rightB)
{
    ledcWrite(chans[0], leftF);     // left-side DC motor
    ledcWrite(chans[1], leftB);
    ledcWrite(chans[2], rightF);      // right-side DC motor
    ledcWrite(chans[3], rightB);
}
