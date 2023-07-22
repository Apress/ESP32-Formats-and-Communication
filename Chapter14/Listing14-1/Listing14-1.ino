/*******************************************************************************
 * Sketch name: Control of ESP32-CAM robot car with direction buttons
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    14 - Remote control ESP32-CAM robot car
 ******************************************************************************/

#include <WiFi.h>// include libraries
WiFiClient client;// associate client and server
WiFiServer server(80);//  WiFiClient and WiFiServer lib
#include <esp_camera.h>
#include <esp_http_server.h>
#include "stream_handler.h"// stream handler function
#include <ssid_password.h>// file with SSID and password
//char ssidAP[] = "ESP32CAM";// softAP SSID and password
//char passwordAP[] = "pass1234";
int servo, oldServo, pulse, duty, servoFreq = 50, servoPin = 2, servoChan = 6;// servo pin and channel
int low = 1500, high = 2500;        // pulses to move to 0 and 90
int pins[] = {12, 13, 15, 14};     // left and right DC motor pins
int chans[] = {1, 2, 3, 4};       // ledc channels 
int freq = 1000, resol = 8;       //     frequency and resolution
String str, reply, direct;
String frames[] = {"96x96","QQVGA 160x120","QCIF 176x144","HQVGA 240x176",
                   "240x240","QVGA 320x240","CIF 400x296","HVGA 480x320",
                   "VGA 640x480","SVGA 800x600","XGA 1024x768","HD 1280x720",
                   "SXGA 1280x1024","UXGA 1600x1200"};
int indexS, indexF, frm, oldfrm = 0;
int speed, slowSpeed = 70;// slow motor speed for turns

void setup()
{
  Serial.begin(115200);// Serial Monitor baud rate
//  WiFi.softAP(ssidAP, passwordAP);// option for softAP
//  Serial.println(WiFi.softAPIP());
  WiFi.begin(ssid, password);// initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());// display server IP address
  configCamera();           // functions to configure camera
  startServer();            //     and start server
  server.begin();// initiate server
  ledcAttachPin(servoPin, servoChan);// match servo pin to channel
  ledcSetup(servoChan, servoFreq, resol);//    and define PWM square wave
  for (int i=0; i<4; i++)
  {
    ledcAttachPin(pins[i], chans[i]);   // match pins[] to channels
    ledcSetup(chans[i], freq, resol);   //    and define PWM square wave
  }
}

void loop()
{
  client = server.available();
  if (client)// initialise client connection
  {
    while (!client.available()) {};  // no client response, do nothing
    str = client.readStringUntil('\r');// map str to message
    Serial.println(str);
    indexS = str.indexOf("frame");// position of "frame" in string
    indexF = str.indexOf("&speed");
    frm = (str.substring(indexS+6, indexF)).toInt();// convert to integer
    if(frm != oldfrm)// image resolution changed
    {
      oldfrm = frm;
      sensor_t * s = esp_camera_sensor_get();
      s->set_framesize(s, (framesize_t)frm);    // update image resolution
    }
    indexS = str.indexOf("speed");// identify maximum motor speed
    indexF = str.indexOf("&servo");
    speed = (str.substring(indexS+6, indexF)).toInt();
    indexS = str.indexOf("servo");// identify servo angle
    indexF = str.indexOf("HTTP");
    servo = (str.substring(indexS+6, indexF-1)).toInt();// servo motor
    if(servo != oldServo)
    {
      pulse = low + (high-low)*servo/90.0;    // square wave pulse length
      duty = round(pulse*servoFreq*(pow(2,resol)-1)/pow(10,6));     // duty cycle
      ledcWrite(servoChan, duty);       // generate square wave
      Serial.printf("image %d speed %d servo %d \n", frm, speed, servo); 
    }     
         if(str.indexOf("R") != -1) {direct = "right"; motor(slowSpeed, 0, 0 ,0);}
    else if(str.indexOf("L") != -1) {direct = "left";  motor(0, 0, slowSpeed, 0);}
    else if(str.indexOf("F") != -1) {direct = "forward";  motor(speed, 0, speed, 0);}
    else if(str.indexOf("B") != -1) {direct = "backward"; motor(0, speed, 0, speed);}
    else if(str.indexOf("S") != -1) {direct = "stop"; motor(0,0,0,0);}
    reply = direct +","+ frames[frm] +","+ String(WiFi.RSSI());
    client.println("HTTP/1.1 200 OK");  // HTTP header starts with response code
    client.println("Content-type:text/html\n");  // \n required for to generate blank line as part response to client
    client.println(reply); // server response
    client.stop();  // close connection
  }
}
            // function to create PWM signal on each channel
void motor(int leftF, int leftB, int rightF, int rightB)
{
    ledcWrite(chans[0], leftF);     // left-side DC motor
    ledcWrite(chans[1], leftB);
    ledcWrite(chans[2], rightF);      // right-side DC motor
    ledcWrite(chans[3], rightB);
}
