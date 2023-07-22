/*******************************************************************************
 * Sketch name: Control two LEDs through a webpage
 * Description: Extend Listing 7-4
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

int LEDG = 0, LEDR, LEDGpin = 13, LEDRpin = 12;  // define LED pins

void setup()            // additional instructions
{               //     to Listing 7-4
  pinMode(LEDGpin, OUTPUT);       // define LED pins as output
  pinMode(LEDRpin, OUTPUT);
  server.on("/LEDGurl", LEDGfunct);     // map URLs to functions
  server.on("/LEDRurl", LEDRfunct);
}

void base()             // same as Listing 7-4

void LEDGfunct()          // function to turn on or off green LED
{
  LEDG = 1 – LEDG;          // alternate LED state
  digitalWrite(LEDGpin, LEDG);      // update LED state
  str = (LEDG == HIGH ? "ON" : "OFF");    // set str to LED state
  server.send(200, "text/plain", str);    // transmit LED state to client
}

void LEDRfunct()            // similar function for red LED
{
  LEDR = !digitalRead(LEDRpin);     // different method to
  digitalWrite(LEDRpin, LEDR);      //    alternate LED state
  str = (LEDR == HIGH ? "ON" : "OFF");
  server.send(200, "text/plain", str);
}

void loop()             // same as Listing 7-4
