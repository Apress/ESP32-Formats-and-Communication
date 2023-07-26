/*******************************************************************************
 * Sketch name: Bluetooth communication
 * Description: Transceive with Bluetooth device
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appBlue()      // function to receive and transmit with Bluetooth
{
  SerialBT.begin("ESP32 Bluetooth");    // initialise Bluetooth
  clrScreen();            // call clear screen function
  String str;
  int count = 0;
  int16_t x, y;
  while (!ttgo->getTouch(x, y))     // wait for touch
  {
    if(SerialBT.available())        // message received
    {
      str = SerialBT.readString();      // convert message to string
      count++;
      if(count > 5)         // check if screen full
      {
        count = 0;          // if so, clear screen
        clrScreen();
      }
      tft->println(str);        // display received message
    }
    if(str.indexOf("battery") != -1)    // check for substring in message
    {               // isChargeing is not a typo
      if (power->isChargeing()) str = "Battery charging";
      else str = "Battery at "+String(power->getBattPercentage())+"%";
      SerialBT.println(str);        // transmit message
    }
  }
  while (ttgo->getTouch(x, y)) {}     // wait for touch release
}

void clrScreen()            // function to clear screen
{
  tft->fillScreen(TFT_BLACK);
  tft->drawBitmap(0, 0, BTimage, 32, 32, TFT_WHITE);  // display icon and text
  tft->setCursor(50, 10);
  tft->setTextColor(TFT_YELLOW);
  tft->print("Bluetooth");
  tft->setTextColor(TFT_GREEN);
  tft->setCursor(0,40);
}
