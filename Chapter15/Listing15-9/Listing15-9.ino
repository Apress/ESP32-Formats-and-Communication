/*******************************************************************************
 * Sketch name: Original sketch
 * Description: Flashes LED with variable flash time
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

const int totalTime = 1000;        // total of LED on and off times
int LEDpin = 4;           // define GPIO pin
int LEDtime = 100;          // initial LED on time

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // LED pin as OUTPUT
}

void loop()
{
  if(Serial.available()) LEDtime = Serial.parseInt(); // new LED on-time
  digitalWrite(LEDpin, HIGH);         // turn on LED
  Serial.printf("HIGH time %d \n",LEDtime);   // display LED on-time
  delay(LEDtime);
  digitalWrite(LEDpin, LOW);          // turn off LED
  delay(totalTime-LEDtime);
}
