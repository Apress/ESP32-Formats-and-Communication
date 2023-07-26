/*******************************************************************************
 * Sketch name: Bluetooth signal to ESP32 microcontroller
 * Description: Transmit audio to ESP32 connected to decoder
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <BluetoothA2DPSink.h>    // include library
BluetoothA2DPSink BT;          // associate BT with library
i2s_pin_config_t pin_config = {
  .bck_io_num = 27,         // BCK (bit clock)
  .ws_io_num = 25,          // LRCK (word select or frame)
  .data_out_num = 26,         // DIO (data input)
  .data_in_num = I2S_PIN_NO_CHANGE};

void setup()
{
  BT.set_pin_config(pin_config);      // update SPI pins
  BT.start("BTmusic");          // establish Bluetooth server
}

void loop()             // nothing in loop function
{}
