/*******************************************************************************
 * Sketch name: Motor driver control
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

int IN1 = 25, IN2 = 26;          // control input pins
int LEDC1 = 1, LEDC2 = 2;       // ledc channels
int pot, potPin = 33;         // potentiometer pin
int minPot = 220, maxPot = 3100;    // min and max potentiometer voltages
int minDuty = 50, maxDuty = 255;    // min and maximum scaled duty cycle
int buffer = 100;           // buffer > noise level
int freq = 1000, resol = 8;       // square wave freq, 8-bit PWM
float neutral, aHigh, aLow, b;      // regression coefficients
int duty, chan, chanNull;

void setup()
{
  ledcAttachPin(IN1, LEDC1);        // allocate ledc channels
  ledcAttachPin(IN2, LEDC2);
  ledcSetup(LEDC1, freq, resol);      // PWM frequency and resolution
  ledcSetup(LEDC2, freq, resol);
  neutral = (maxPot + minPot)/2.0;      // middle of potentiometer range
  b = (maxDuty - minDuty)/((maxPot - minPot)/2.0 - buffer);
  aHigh = maxDuty - b * maxPot;     // regression coefficients for high
  aLow  = maxDuty + b * minPot;     // and low potentiometer voltages
}

void loop()
{
  pot = analogReadMilliVolts(potPin);   // potentiometer voltage
  pot = constrain(pot, minPot, maxPot);   // constrained between limits
  motor();              // call motor function
}

void motor()      // function to control direction and speed of rotation
{             // high or low potentiometer voltage
  if(pot >= neutral + buffer) duty = round(aHigh + b * pot);
  else if(pot <= neutral - buffer) duty = round(aLow  - b * pot);
  else duty = 0;          // potentiometer voltage in neutral zone
  chan = 1 + int(pot/neutral);      // channel with PWM signal
  chanNull = 3 - chan;
  ledcWrite(chan, duty);        // generate PWM signal
  ledcWrite(chanNull, 0);       // channel with no signal
}
