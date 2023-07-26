/*******************************************************************************
 * Sketch name: Dashboard
 * Description: Display sensor data from remote ESP32
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <ESPDash.h>          // include ESPDash library
AsyncWebServer server(80);    // associate server with ESPAsyncWebServer
ESPDash dashboard(&server);     //    and dashboard with ESPDash
char ssidAP[] = "ESP32_Dashboard";    // microcontroller as access point
char passwordAP[] = "pass1234";
Card textCard(&dashboard, GENERIC_CARD, "Changing text");   // text
Card rateCard(&dashboard, GENERIC_CARD, "Rate", "%");     // rate%
Card tempCard(&dashboard, TEMPERATURE_CARD, "Water temp", "°C");  // tempC
Card stateCard(&dashboard, STATUS_CARD, "Current state");   // state
Card progCard(&dashboard, PROGRESS_CARD, "Distance", "cm", 1, 100);
Card slider(&dashboard, SLIDER_CARD, "Slider", " LED bright", 0, 255);
Card button(&dashboard, BUTTON_CARD, "LED on ESP32");    // LED
Chart graph(&dashboard, BAR_CHART, "Random values");
int LEDon, LEDpin = 2;            // ESP32 built-in LED
String state[] = {"idle","warning","success","danger"};
String message[] = {"rate low","rate medium","rate high","rate extreme"};
String Xaxis[] = {"N-5","N-4","N-3","N-2","N-1","NOW"};
int Yaxis[] = {0,0,0,0,0,0};
char chr[8];        // 7 letters + 1 as "warning" is the longest string
unsigned long lag = 0;
int channel = 0, freq = 5000, resolution = 8;   // LED PWM parameters
int value, LEDpwm, graphN = 0;
float temp;
String str;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);
  ledcAttachPin(LEDpin, channel);     // PWM parameters for LED
  ledcSetup(channel, freq, resolution);
  WiFi.softAP(ssidAP, passwordAP);      // connection to softAP device
  Serial.println(WiFi.softAPIP());      // display SoftAP address
  server.begin();           // initialise server
  button.attachCallback([&](bool LEDval)    // attach button callback
  {
    button.update(LEDval);        // update button card
    Serial.println("button "+String((LEDval) ? "on" : "off"));
    LEDon = LEDval;
  });
  slider.attachCallback([&](int slideVal)   // attach silder callback
  {
    slider.update(slideVal);        // update slider
    Serial.println("Slider "+String(slideVal));
    LEDpwm = slideVal;
  });
  graph.updateX(Xaxis, 6); // define X-axis with 6 values
}

void loop()
{
  if(millis() > lag + 2000) // interval between updates
  {
    lag = millis();
    if(LEDon == 0) ledcWrite(channel, 0);   // turn LED off if button is off
    else ledcWrite(channel, LEDpwm);      // update LED brightness
    value = random(1, 100);       // generate random number
    graphN++;
    if(graphN > 4)  // update graph every 5 cycles
    {
      for (int i=0; i<5; i++) Yaxis[i] = Yaxis[i+1]; // shift data values one position
      Yaxis[5] = value;  // update graph data
      graph.updateY(Yaxis, 6);  // Y-axis data with 6 values
      graphN = 0; // reset counter
    }
    str = "rate is " + String(value);     // append value to string
    textCard.update(str);         // update generic card with string
    rateCard.update(value);       //     generic card with integer,
    temp = value * 0.5;
    tempCard.update(temp);        //     temperature card with float,
    progCard.update(value);       //     progress card with integer
    value = value/25;       // convert value to one of four categories
    state[value].toCharArray(chr, 8);     // convert string to char array
    stateCard.update(message[value], chr);  // update state card message and image
    dashboard.sendUpdates();        // update dashboard
  }            
}
