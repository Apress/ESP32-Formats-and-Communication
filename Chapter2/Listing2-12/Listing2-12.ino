/*******************************************************************************
 * Sketch name: Bluetooth signal to M5Stack Core2
 * Description: Transmit audio to M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>          // include M5Core2 and
#include <Audio.h>          //    ESP32-audioI2S libraries
Audio audio;            // associate audio with library
int BCLK = 12, LRC = 0, DOUT = 2;     // define I2S GPIO pins
#include <BluetoothA2DPSink.h>
BluetoothA2DPSink BT;

void setup()
{
  M5.begin();           // initialise M5Stack module
  M5.Axp.SetSpkEnable(true);        // enable speaker
  i2s_pin_config_t pin_config = {
  .bck_io_num = BCLK,         // BCK (bit clock)
  .ws_io_num = LRC,         // LRCK (word select or frame)
  .data_out_num = DOUT,         // DIO (data input)
  .data_in_num = I2S_PIN_NO_CHANGE
  };
  BT.set_pin_config(pin_config);
  BT.start("BTmusic");
  audio.setPinout(BCLK, LRC, DOUT);     // initialise audio
  delay(500);
}

void loop()
{
  audio.loop();           // manage audio streaming
}
