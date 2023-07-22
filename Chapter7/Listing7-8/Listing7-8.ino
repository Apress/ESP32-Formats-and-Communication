/*******************************************************************************
 * Sketch name: WebSocket wsEvent function
 * Description: webSocket extenson to Listing 7-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

void wsEvent(uint8_t n, WStype_t type, uint8_t * message, size_t length)
{
  if(type == WStype_TEXT)
  {
    str = "";            // convert message to string
    for (int i=0; i<length; i++) str = str + char(message[i]);
    LEDpin = (str == "LEDG" ? LEDGpin : LEDRpin); // determine required LED
    digitalWrite(LEDpin, !digitalRead(LEDpin));   // turn on or off LED
    readLED();              // call readLED function
  }
}
