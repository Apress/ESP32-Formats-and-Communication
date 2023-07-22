/*******************************************************************************
 * Sketch name: Bitmap of additional font
 * Description: Rock_Salt_Regular_24 font
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

const uint8_t Rock_Salt_Regular_24Bitmaps[] PROGMEM = {
// Bitmap Data:
0x00, // ' '
⋮ 
{  5400,   4,  25,   8,    1,  -20 }, // '|'
{  5413,  14,  32,  15,   -2,  -22 } // '}'
};
const GFXfont Rock_Salt_Regular_24 PROGMEM = {
(uint8_t  *)Rock_Salt_Regular_24Bitmaps,
(GFXglyph *)Rock_Salt_Regular_24Glyphs,0x20, 0x7E, 58};
