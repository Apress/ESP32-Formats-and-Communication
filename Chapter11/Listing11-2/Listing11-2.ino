/*******************************************************************************
 * Sketch name: ESP32 CAM and ILI9341 TFT LCD screen
 * Description: Display images on connected LCD
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#include <esp_camera.h>          // include camera library and
#include "config_pins.h"        // camera configure pins tab
#include <TJpg_Decoder.h>       // include JPEG decoder library
#include <TFT_eSPI.h>         // include TFT_eSPI library,
TFT_eSPI tft = TFT_eSPI();        //     associate tft with library

void setup()
{
  configCamera();           // function to configure camera
  tft.begin();            // initialise TFT LCD screen
  tft.setRotation(3);         // portrait orientation
  tft.fillScreen(TFT_BLACK);
  TJpgDec.setJpgScale(1);       // jpeg image scale factor
  TJpgDec.setSwapBytes(true);       // convert image byte order
  TJpgDec.setCallback(tftOutput);     // call tftOutput function
}

bool tftOutput            // function to decode JPEG image
  (int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if (y >= tft.height()) return 0;      // constrain displayed image
  tft.pushImage(x, y, w, h, bitmap);    // clip image to screen boundaries
  return 1;             // return 1 to decode next block
}

void loop()
{
  camera_fb_t * frame = esp_camera_fb_get();  // get image from camera
  TJpgDec.drawJpg(0,0,(const uint8_t*) frame->buf, frame->len);
  esp_camera_fb_return(frame);      // return frame buffer to driver
}
