/*******************************************************************************
 * Sketch name: WS2812 colour selection with Bluetooth
 * Description: Alternative to Wi-Fi in Listing 12-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    12 - Control apps
 ******************************************************************************/

#include <BluetoothSerial.h>        // include BluetoothSerial library
BluetoothSerial SerialBT;
#include <Adafruit_NeoPixel.h>      // include NeoPixel library
int LEDpin = 25;            // define data pin
int LEDnumber = 12;         // number of LEDs on ring
int32_t color;            // color is 32-bit or signed long
              // associate ring with NeoPixel library
Adafruit_NeoPixel ring(LEDnumber, LEDpin, NEO_GRB + NEO_KHZ800);
String str, substr;
char c;
int R,G,B, indexS, indexF;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  SerialBT.begin("ESP32 Bluetooth");    // initialise Bluetooth
  ring.begin();
  ring.setBrightness(1);        // LED ring brightness
  ring.show();
}

void loop()
{
  if(SerialBT.available())        // character in Bluetooth buffer
  {
    str = "";           // reset string
    while(SerialBT.available()>0)
    {
      c = SerialBT.read();        // accumulate buffer to a string
      str = str + String(c);
    }
    Serial.println(str);        // display string on Serial Monitor
    indexS = 4;           // parse red colour component
    indexF = str.indexOf("G");      // between space after "Red" and
    substr = str.substring(indexS, indexF); // the letter G of "Green"
    R = substr.toInt();
    indexS = indexF + 6;        // parse green colour component
    indexF = str.indexOf("B");      // between space after "Green"
    substr = str.substring(indexS, indexF); // and the letter B of "Blue"
    G = substr.toInt();
    indexS = indexF + 5;        // parse blue colour component
    substr = str.substring(indexS);     // following space after "Blue"
    B = substr.toInt();
    Serial.printf("RGB: %d %d %d \n", R, G, B); // display colour components
    color = ring.Color(R, G, B);      // convert RGB values to colour
    ring.fill(color);         // set the LED colour
    ring.show();            // update LED ring colour
  }
}
