/*******************************************************************************
 * Sketch name: remote ILI9341 TFT LCD screen - server
 * Description: Display images from remote ESP32-CAM
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/
#include <M5Core2.h>
#include <TJpg_Decoder.h>
//#include <TFT_eSPI.h>
//TFT_eSPI tft = TFT_eSPI();
#include <WebSocketsServer.h>             // WebSocket server library
WebSocketsServer websocket = WebSocketsServer(81);  // WebSocket on port 81
char ssidAP[] = "ESP32CAM";
char passwordAP[] = "pass1234";
unsigned long last;
int FPS, FPSs[50], sum = 0, N = 0;        // circular buffer array
float mean;

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssidAP, passwordAP);        // initialise softAP WLAN
  Serial.print("server softAP address ");
  Serial.println(WiFi.softAPIP());        // server IP address
  websocket.begin();            // initialise WebSocket
  websocket.onEvent(wsEvent);         // wsEvent called by
  M5.begin();
//  tft.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tftOutput);
  M5.Lcd.setTextColor(TFT_RED);          // font colour display FPS
  M5.Lcd.setTextSize(2);
  for (int i=0; i<50; i++) FPSs[i] = 0;     // initialise circular buffer
}

bool tftOutput (int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if (y >= M5.Lcd.height()) return 0;
  M5.Lcd.pushImage(x, y, w, h, bitmap);
  return 1;
}
              // function called on WebSocket event
void wsEvent(uint8_t num, WStype_t type, uint8_t * RXmsg, size_t length)
{
  last = millis();          // update last image time
  TJpgDec.drawJpg(0,0, RXmsg, length);    // display image on screen
  FPS = millis() - last;        // interval between images
  circular();           // update circular buffer
  M5.Lcd.setCursor(220,220);       // position cursor bottom right
  M5.Lcd.print("FPS ");M5.Lcd.print(mean, 1);   // display FPS
}

void circular()           // function for circular buffer
{
  sum = sum - FPSs[N];          // subtract oldest value from sum
  sum = sum + FPS;          // add current value to sum
  FPSs[N] = FPS;            // update circular buffer
  N++;              // increment buffer position
  if(N > 50-1) N = 0;         // back to "start" of circular buffer
  mean = 1000.0/(sum/50.0);       // moving average FPS
}

void loop()
{
  websocket.loop();         // handle WebSocket data
}
