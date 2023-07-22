/*******************************************************************************
 * Sketch name: WebSocket event
 * Description: Function for Listing 11-12
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

void wsEvent(uint8_t n, WStype_t type, uint8_t * message, size_t length)
{
  if(type == WStype_TEXT)
  {
    str = "";            // build string from characters
    for (int i=0; i<length; i++) str = str + char(message[i]);
    comma1 = str.indexOf(",");      // comma positions
    comma2 = str.lastIndexOf(",");
    text[0] = str.substring(0, comma1);   // substrings between commas
    text[1] = str.substring(comma1+1, comma2);
    Nservo = text[0].toInt();       // convert string to integer
    Nlight = text[1].toInt();
    Ndirect = str.substring(comma2+1);
    if(Nlight != light)
    {
      light = Nlight;
      digitalWrite(lightPin, light);    // update LED state
      Serial.printf("light %d \n", light);
    }
    if(Nservo != servo)
    {
      servo = Nservo;
      duty = round((2.5+servo/18.0)*(pow(2,resol)-1)/100.0);
      ledcWrite(channel, duty);     // update servo position
      Serial.printf("servo %d pulse %d \n", servo, duty);
    }
    if(Ndirect != direct)
    {
       direct = Ndirect;        // update direction text
       Serial.printf("direction %s \n", direct);
    }
  }
}
