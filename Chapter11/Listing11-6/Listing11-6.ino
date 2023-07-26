/*******************************************************************************
 * Sketch name: ArduinoWebSocket library - server 
 * Description: WebSocket extension to Listing 11-5
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

void loop()
{
  if(server.poll()) client = server.accept();
  if(client.available())
  {
    WebsocketsMessage RXmsg = client.readBlocking();
    last = millis();
    TJpgDec.drawJpg(0,0,(const uint8_t*) RXmsg.c_str(), RXmsg.length());
    FPS = millis() - last;
    circular();
    tft.setCursor(220,220);
    tft.print("FPS ");tft.print(mean, 1);
  }
}
