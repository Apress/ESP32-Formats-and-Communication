/*******************************************************************************
 * Sketch name: Servo motor control 
 * Description: Table 13-2 with ledc
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

int channel = 0;
int servoPin = 5;
int angle, pulse, freq = 50, duty, resol = 8;
int low = 350, high = 2150;

void setup()
{
  ledcAttachPin(servoPin, channel);
  ledcSetup(channel, freq, resol);
}

void loop()
{
  for (int i=1; i<4; i++)
  {
    angle = 45*i;
    pulse = low + (high-low)*angle/180;
    duty = pulse*freq*(pow(2,resol)-1)*pow(10,-6);
    ledcWrite(channel, duty);
    delay(1000);
  }
}
