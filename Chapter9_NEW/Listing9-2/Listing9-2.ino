/*******************************************************************************
 * Sketch name: TVOC and CO2 measurement and Blynk MQTT broker
 * Description: Extend Listing 9-1 to include MQTT broker
 * Created on:  September 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    9 - MQTT
 ******************************************************************************/
#define BLYNK_TEMPLATE_ID   "TMPL5lCgtwBmn"
#define BLYNK_TEMPLATE_NAME "Listing 92 template"
#define BLYNK_AUTH_TOKEN    "JdkHtH_14m2T3KK-h9yF_CUEAeo8kXF4"
#define BLYNK_PRINT Serial

#include <Wire.h>            // include Wire and
#include <ccs811.h>;          // CCS811 libraries
#include <BlynkSimpleEsp32.h>      // Blynk MQTT library
#include <ssid_password.h>        // file with logon details

CCS811 ccs811(19);          // CCS811 nWAKE on GPIO 19
uint16_t CO2, TVOC, errstat, rawdata;
int LEDpin = 2;           // LED as activity indicator
int LEDMQTTpin = 4;         // LED controlled with MQTT
int count = 0, countDown = 200;
unsigned long LEDtime = 0, countTime;

BlynkTimer timer;
void timerEvent()
{
  ccs811.read(&CO2, &TVOC, &errstat, &rawdata); // read CCS811 sensor data
  if(errstat == CCS811_ERRSTAT_OK)      // given valid readings
  {
    count++;            //increment reading counter
    countDown = 60;
    countTime = millis();       // set countdown time
    Blynk.virtualWrite(V3, CO2);  // send data to MQTT
    Blynk.virtualWrite(V4, TVOC);
    Blynk.virtualWrite(V6, count);
    Blynk.virtualWrite(V7, countDown);
    Serial.printf("count %d \n", count);
    LEDtime = millis();
    digitalWrite(LEDpin, HIGH);     // turn on indicator LED
  }               // turn off LED after 100ms
  if(millis() - LEDtime > 100) digitalWrite(LEDpin, LOW);
  if(millis() - countTime > 5000)     // countdown interval 5s
  {
    countTime = millis();       // update countdown time
    countDown = countDown - 5;      // update countdown
    Blynk.virtualWrite(V7, countDown); // send to MQTT
  }
}

BLYNK_WRITE(V0)   // Blynk virtual pin 0
{
  digitalWrite(LEDMQTTpin, param.asInt()); // turn on or off LED
}

BLYNK_CONNECTED()  // function called when ESP32 connected
{
  Serial.println("ESP32 connected to Blynk");
}

void setup()
{
  Serial.begin(115200);            // initiate Blynk MQTT
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, "blynk.cloud", 80);
  timer.setInterval(1000, timerEvent);  // timer event called every 1s
  pinMode(LEDpin, OUTPUT);        // define LED pins as output
  digitalWrite(LEDpin, LOW);        // turn off LEDs
  pinMode(LEDMQTTpin, OUTPUT);
  digitalWrite(LEDMQTTpin, LOW);
  Wire.begin();           // initialise I2C
  ccs811.begin();           // initialise CCS811
  ccs811.start(CCS811_MODE_60SEC);      // set reading interval at 60s
}

void loop()
{
  Blynk.run();    // maintains connection to Blynk
  timer.run();    // Blynk timer
}
