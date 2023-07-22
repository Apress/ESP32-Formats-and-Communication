/*******************************************************************************
 * Sketch name: DC motor speed and Hall effect sensor
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

int IN1 = 25, IN2 = 26;          // DC motor control input pins
int chan = 1, chanNull = 2;       // ledc channel numbers
int duty, freq = 1000, resol = 8;     // PWM frequency, 8-bit PWM
unsigned long revol, last = 0;
int hallThresh = 60;           // Hall effect threshold
int hall, hallState = LOW, oldState = LOW, RPM;
int LEDpin = 2;           // ESP32 module LED pin

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  ledcAttachPin(IN1, chan);       // allocate ledc channels to GPIO
  ledcAttachPin(IN2, chanNull);
  ledcSetup(chan, freq, resol);     // square wave for each channel
  ledcSetup(chanNull, freq, resol);
  pinMode(LEDpin, OUTPUT);        // define LED pin as OUTPUT
}

void loop()
{
  if(Serial.available())        // value entered on Serial Monitor
  {
    duty = Serial.parseInt();       // convert to integer
    ledcWrite(chan, duty);        // generate PWM signal
    ledcWrite(chanNull, 0);       // channel with no signal
  }
  hall = hallRead();          // read Hall effect sensor
  if(hall < hallThresh) hallState = LOW;    // low Hall effect value detected
  else hallState = HIGH;        // Hall effect value > threshold
  if(hallState == HIGH && oldState == LOW)  // change in state
  {
    revol = millis() - last;        // revolution time (ms)
    if(revol < 100) return;       // 100 ms equivalent to 600 RPM
    last = millis();          // update time of last state change
    RPM = 60000.0/revol;        // determine motor speed
    Serial.printf("duty %d revol %d RPM %d \n", duty, revol, RPM);
    digitalWrite(LEDpin, !digitalRead(LEDpin)); // alternate LED state
  }
  oldState = hallState;         // update change state
}
