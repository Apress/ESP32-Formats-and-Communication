/*******************************************************************************
 * Sketch name: Servo motor control
 * Description: Table 13-2 with ESP32Servo
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

#include <ESP32Servo.h>
Servo servo;
int servoPin = 5;
int angle, pulse, freq = 50;
int low = 350, high = 2150;

void setup()
{
  servo.attach(servoPin, low, high);
  servo.setPeriodHertz(freq);
}

void loop()
{
  for (int i=1; i<4; i++)
  {
    angle = 45*i;
    pulse = low + (high-low)*angle/180;
    servo.write(angle);
    delay(500);
    servo.writeMicroseconds(pulse);
    delay(1000);
  }
}
