/*******************************************************************************
 * Sketch name: Expanded sketch
 * Description: Expansion of Listing 15-9
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

const int totalTime = 1000;
class flashLibrary
{
  public:
    flashLibrary(int pin, int time)    // constructor has same name as library
    {
      LEDpin = pin;         // LEDpin and LEDtime are
      LEDtime = time;         // local variables to the library
    }
    void begin(int baud = 115200)     // function called by main sketch
    {               // with a default baud rate
      Serial.begin(baud);
      pinMode(LEDpin, OUTPUT);
    }
    void flashLED()         // function called by main sketch
    {
      if(Serial.available()) LEDtime = Serial.parseInt();
      digitalWrite(LEDpin, HIGH);
      Serial.printf("HIGH time %d \n",LEDtime);
      delay(LEDtime);
      digitalWrite(LEDpin, LOW);
      delay(totalTime-LEDtime);
    }
  private:              // variables local to library
    int LEDpin;
    int LEDtime;
};                // note semi-colon

flashLibrary flash(4, 100);     // associate flash with library
void setup()          // and pass GPIO pin and LED time
{
  flash.begin();          // main sketch calls begin function
}
void loop()
{
  flash.flashLED();       // main sketch calls flashLED function
}
