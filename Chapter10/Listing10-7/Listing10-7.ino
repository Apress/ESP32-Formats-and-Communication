/*******************************************************************************
 * Sketch name: Image X bitmap data file
 * Description: Snowflake Xbitmap
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

int flakeW = 13, flakeH = 13;
const unsigned char snowflake[] PROGMEM = {
  0x08, 0x02, 0x18, 0x03, 0xB8, 0x03, 0xB7, 0x1D, 0xAE, 0x0E,
  0x1C, 0x07, 0x40, 0x00, 0x1C, 0x07, 0xAE, 0x0E, 0xB7, 0x1D,
  0xB8, 0x03, 0x18, 0x03, 0x08, 0x02
};
