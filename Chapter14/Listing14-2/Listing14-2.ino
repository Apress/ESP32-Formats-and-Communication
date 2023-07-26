/*******************************************************************************
 * Sketch name: Control of ESP32-CAM robot car with heading angle 
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    14 - Remote control ESP32-CAM robot car
 ******************************************************************************/

#include <WiFi.h>
WiFiClient client;
WiFiServer server(80);
#include <esp_camera.h>
#include <esp_http_server.h>
#include "stream_handler.h"
#include <ssid_password.h>
int servo, oldServo, pulse, duty
int servoFreq = 50, servoPin = 2, servoChan = 6;
int low = 1500, high = 2500;
int pins[] = {12, 13, 15, 14};
int chans[] = {1, 2, 3, 4};
int freq = 1000, resol = 8;
String str, reply;
String frames[] = {"96x96","QQVGA 160x120","QCIF 176x144","HQVGA 240x176",
                   "240x240","QVGA 320x240","CIF 400x296","HVGA 480x320",
                   "VGA 640x480","SVGA 800x600","XGA 1024x768",
                   "HD 1280x720","SXGA 1280x1024","UXGA 1600x1200"};
int indexS, indexF, frm, oldfrm = 0;
int minSpeed = 50, maxSpeed;        // min and max. motor speeds
float a, b, scalar = 2.0, buffer = 0.1;
float sinDeg, cosDeg, FB, LR, Sum, Dif;
int degree;
int motorR, motorL, chanR, chanRnull, chanL, chanLnull;
String directFB, directLR;
int LEDval = 0, LEDpin = 4;       // COB LED on GPIO 4

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  configCamera(); 
  startServer();
  server.begin();
  ledcAttachPin(servoPin, servoChan);   
  ledcSetup(servoChan, servoFreq, resol);
  for (int i=0; i<4; i++)
  {
    ledcAttachPin(pins[i], chans[i]);
    ledcSetup(chans[i], freq, resol);
  }
  pinMode(LEDpin, OUTPUT);        // LED pin as output
}

void loop()
{
  client = server.available();
  if (client)
  {
    while (!client.available()) {};
    str = client.readStringUntil('\r');
    Serial.println(str);
    splitStr();           // call function to parse string
    if(frm != oldfrm)
    {
      oldfrm = frm;
      sensor_t * s = esp_camera_sensor_get();
      s->set_framesize(s, (framesize_t)frm);
    }
    if(servo != oldServo)
    {
      oldServo = servo;
      pulse = low + (high-low)*servo/90.0;
      duty = round(pulse*servoFreq*(pow(2,resol)-1)/pow(10,6));
      ledcWrite(servoChan, duty); 
    }
    reply = frames[frm] +","+ String(WiFi.RSSI());
    if(degree != 999)         // updated heading angle
    {               // call function to convert
      convert();            //   heading angle to motor speeds
      reply = reply +","+ directFB +" "+ directLR;
      reply = reply +","+ String(motorL) +" "+ String(motorR);
    }
    else
    {
      motor(0,0,0,0);         // stop DC motors
      reply = reply +", , ";        // no directions or speeds in reply
    }
    digitalWrite(LEDpin, LEDval);     // update COB LED state
    Serial.println(reply);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html\n");
    client.println(reply);
    client.stop();
  }
}

void splitStr()         // function to parse HTTP GET request
{
  indexS = str.indexOf("degree");   // index of starting substring position
  indexF = str.indexOf("&frame");   // index of final substring position
  degree = (str.substring(indexS+7, indexF)).toInt();
  indexS = str.indexOf("frame");
  indexF = str.indexOf("&speed");
  frm = (str.substring(indexS+6, indexF)).toInt();
  indexS = str.indexOf("speed");
  indexF = str.indexOf("&servo");
  maxSpeed = (str.substring(indexS+6, indexF)).toInt();
  indexS = str.indexOf("servo");
  indexF = str.indexOf("&LED");
  servo = (str.substring(indexS+6, indexF)).toInt();
  indexS = str.indexOf("LED");
  indexF = str.indexOf("HTTP");
  LEDval = (str.substring(indexS+4, indexF-1)).toInt();
}

void convert()        // function to convert degree to DC motor speeds
{
    sinDeg = sin(DEG_TO_RAD*degree);      // point co-ordinates from
    cosDeg = cos(DEG_TO_RAD*degree);        //    heading angle
    FB = 0;
    LR = 0;
    if(abs(sinDeg) > buffer)        // forward-backward component
       FB = minSpeed + (maxSpeed - minSpeed) * abs(sinDeg);
    if(abs(cosDeg) > buffer)          // left-right component
       LR = minSpeed + (maxSpeed - minSpeed) * abs(cosDeg);
    Sum = FB + LR/scalar;         // sum and difference of
    Dif = FB - LR/scalar;         //  FB and LR components
//    if(Sum > maxSpeed) Sum = maxSpeed;
    adjust();           // function to derive regression coefficients
    Sum = a + Sum * b;            // reduced Sum
    if(Dif < minSpeed) Dif = 0;       // restrict Dif
    directLR = "";
    directFB = "";
    if(cosDeg > buffer) directLR = "right";   // motor direction
    else if(cosDeg < -buffer) directLR = "left";  //    for display on app
    if(sinDeg > buffer) directFB = "forward";
    else if(sinDeg < -buffer) directFB = "backward";
    motorL = 0;
    motorR = 0;           // map motor speed to each motor
    if(cosDeg < 0) {motorR = round(Sum); motorL = round(Dif);}
    else           {motorR = round(Dif); motorL = round(Sum);}
    if(sinDeg < 0) motor(0, motorL, 0,  motorR);    // move backward
    else          motor(motorL, 0,  motorR, 0);     // move forward
}

void adjust()       // function to determine regression coefficients
{
  float maxAngle, maxFB, maxLR, maxSum;
  maxAngle = atan(scalar)*180.0/PI;          // angle for maximum value
  maxFB = minSpeed + (maxSpeed - minSpeed) * sin(DEG_TO_RAD*maxAngle);
  maxLR = minSpeed + (maxSpeed - minSpeed) * cos(DEG_TO_RAD*maxAngle);
  maxSum = maxFB + maxLR/scalar;           // maximum function value
  b = (maxSpeed - minSpeed)/(maxSum - minSpeed);  // gradient or slope
  a = minSpeed * (1.0 - b);         // intercept
}

void motor(int leftF, int leftB, int rightF, int rightB)
{
    ledcWrite(chans[0], leftF);
    ledcWrite(chans[1], leftB);
    ledcWrite(chans[2], rightF);
    ledcWrite(chans[3], rightB);
}
