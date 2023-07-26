/*******************************************************************************
 * Sketch name: Servo motor calibration
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

#include <ESP32Servo.h>          // Servo library for ESP32
Servo servo;            // associate servo with library
int servoPin = 5;           // servo pin
int pulse;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  servo.attach(servoPin);       // initialise servo motor
}

void loop()
{
  if(Serial.available())        // text entered in Serial Monitor
  {
    pulse = Serial.parseInt();      // parse text to integer
    servo.writeMicroseconds(pulse);     // move servo motor
  }
}
