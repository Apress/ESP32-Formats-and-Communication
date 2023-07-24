/*******************************************************************************
 * Sketch name: Internet radio with M5Stack Core2
 * Description: Comprehensiove radio with several functions
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>
#include <ssid_password.h>
#include <Audio.h>
Audio audio;
int BCLK = 12;
int LRC = 0;
int DOUT = 2;
unsigned long last = 0;
uint16_t colour;
int vol = 10;   // 0...21
int level;
float batt;
String title, newStation, temp;
const int maxStation = 6;
int station = 0;
// char const * name[N] = {....}  required by C++
char const * URL[maxStation] = {
"media-ice.musicradio.com:80/ClassicFMMP3",
"1940sradio1.co.uk:8100/stream/1/",
"irmedia.streamabc.net/irm-bbrberlinclub-mp3-128-4574783", // Berlin station = 2
//"stream.oneplay.no/drammen128",
"stream.oneplay.no/oslo128", // Oslo
//"stream.radiometro.no/metro128.mp3",
"radio.virginradio.co.uk/stream",
//"stream.live.vc.bbcmedia.co.uk/bbc_radio_fourfm", // BBC radio 4
"dispatcher.rndfnk.com/br/br3/live/aac/low?aggregator=radio-de" // Bayern3
};

void setup()
{
  M5.begin(true, true, true, true);  // LCD, TF Card, Serial port and I2C port
  M5.Axp.SetSpkEnable(true);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.println("connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (!WiFi.isConnected()) delay(500);
  M5.Lcd.println("WiFi connected");
  M5.Lcd.print(WiFi.localIP());
  delay(2000);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Battery ", 0, 155);
  M5.Lcd.drawString("Volume ", 0, 185);
  M5.Lcd.drawString("WiFi signal ", 0, 215);
  displayBatt();
  audio.setPinout(BCLK, LRC, DOUT);
  audio.setVolume(vol);
  delay(500);
  audio.connecttohost(URL[0]);
}

void loop()
{
  audio.loop();
  if (M5.BtnA.wasPressed()) // isPressed: button state last time button read
  {                         // wasPressed: returns 1 when button pressed
    vol++;
    if(vol > 21) vol = 0;
    audio.setVolume(vol);
    M5.Lcd.fillRect(140,185,30,20,BLACK);
    M5.Lcd.setTextColor(WHITE, BLACK);  
    M5.Lcd.drawString(String(vol),140,185);
  }
  if (M5.BtnB.wasPressed())
  {
    station++;
    if(station > maxStation-1) station = 0;
    audio.stopSong();
    audio.connecttohost(URL[station]);
    M5.Lcd.fillRect(0,0,319,20,BLACK);  // some stations do not provide name
    temp = String(URL[station]);
    M5.Lcd.setTextColor(YELLOW, BLACK);
    if(temp.indexOf("virgin") != -1) M5.Lcd.drawString("Virgin Radio",0,0);
  }
  if (M5.BtnC.wasPressed())
  {
    station--;
    if(station < 0) station = maxStation-1;
    audio.stopSong();
    audio.connecttohost(URL[station]);
    M5.Lcd.fillRect(0,0,319,20,BLACK);
    temp = String(URL[station]);
    M5.Lcd.setTextColor(YELLOW, BLACK);
    if(temp.indexOf("virgin") != -1) M5.Lcd.drawString("Virgin Radio",0,0);
  }
  M5.update(); // better after "if (M5.BtnX.wasPressed())" instruction
  if(Serial.available())   // enter streamURL in serial monitor
  {                        // useful to check new URLs
    audio.stopSong();
    newStation = Serial.readString(); 
    audio.connecttohost(newStation.c_str());
  }
  if(millis() - last > 60000) displayBatt(); // battery voltage at 60s intervals
}

void displayBatt()
{
    M5.Lcd.fillRect(140,215,40,20,BLACK); //  blank out previous RSSI
    colour = (WiFi.RSSI() < -70) ? RED : GREEN;  // display low RSSI in red
    M5.Lcd.setTextColor(colour, BLACK);  
    M5.Lcd.drawString(String(WiFi.RSSI()),140,215);
    batt = M5.Axp.GetBatVoltage();
    M5.Lcd.fillRect(140,155,50,20,BLACK); //  blank out previous battery voltage
    colour = (batt < 3.5) ? RED : GREEN;
    M5.Lcd.setTextColor(colour, BLACK); 
    M5.Lcd.drawString(String(batt),140,155);
    level = (M5.Axp.GetBatVoltage() - 3.2)/0.1;  // battery level 3.2 - 4.1V
    for (int i=0; i<9; i++)
    {
      colour = GREEN;  // 3.8-4.1V
      if(i<3) colour = RED;  // 3.2-3.4V
      else if(i<6) colour = YELLOW;  // 3.5-3.7V
      M5.Lcd.fillRoundRect(307,(230-(i*10)),12,7,2,(level>i) ? colour : BLACK);
      M5.Lcd.drawRoundRect(307,(230-(i*10)),12,7,2,TFT_LIGHTGREY);    
    }
    last = millis();
}

/*void battLevel()
{
//  int level = map(M5.Axp.GetBatVoltage() * 100, minVolts, maxVolts, 0 , 8);
  level = (M5.Axp.GetBatVoltage() - 3.2)/0.1;  // battery level 3.2 - 4.1V
  for (int i=0; i<9; i++)
  {
    colour = GREEN;  // 3.8-4.1V
    if(i<3) colour = RED;  // 3.2-3.4V
    else if(i<6) colour = YELLOW;  // 3.5-3.7V
// a bar is 12 pixels wide and 7 pixels high with 3 pixels beteeen bars
    M5.Lcd.fillRoundRect(307,(230-(i*10)),12,7,2,(level>i) ? colour : BLACK);
    M5.Lcd.drawRoundRect(307,(230-(i*10)),12,7,2,TFT_LIGHTGREY);
// for full height of screen, 10 bars 21 pixels high with 3 pixels beteeen bars
// for (int x = 9; x >= 0; x--)
// M5.Lcd.drawRoundRect(314, (216 - (x * 24)), 6, 21, 2, TFT_LIGHTGREY);    
  }
}*/

// see https://cplusplus.com/reference/string/string/c_str/

void splitText(String text)
{
  int line = 25, spaces[50], Nspaces = 0, firstC = 0, lastC = 1;
  String str = text+" ";  // set last space at end of text
  int len = str.length();
  spaces[0] = -1;
  for (int i=0; i<50; i++)
  {
    if(spaces[i]+1 < len)  // not at end of text
    {
      spaces[i+1] = str.indexOf(" ",spaces[i]+1); // locate next space
      Nspaces++;
    }
    else i = 50; // found all spaces
  }
  for (int i=1; i<Nspaces+1; i++)
  {
    if(spaces[i]-lastC > line)  // current space past line length
    {
      lastC = spaces[i-1]; // finish position set to previous space
      M5.Lcd.println(str.substring(firstC, lastC)); // display text between spaces
      firstC = lastC+1; // finish position set to previous space+1
    }
  }
  M5.Lcd.println(str.substring(firstC, len-1));  // display from last space to end of text
}

void audio_showstation(const char *info)
{
  Serial.print("station     ");Serial.println(info);
  M5.Lcd.fillRect(0,0,319,20,BLACK);
  M5.Lcd.setTextColor(YELLOW, BLACK);
  title = String(info);
  M5.Lcd.drawString(title,0,0);
  M5.Lcd.fillRect(0,40,319,80,BLACK); // blank previous track as virgin no track
}

void audio_showstreamtitle(const char *info)
{
  Serial.print("streamtitle ");Serial.println(info);
  M5.Lcd.fillRect(0,40,319,80,BLACK);  // blank title
  M5.Lcd.setTextColor(TFT_ORANGE, BLACK);
  title = String(info);
  M5.Lcd.setCursor(0, 40); // drawString doesn't use setTextWrap
//  if(station != 2) M5.Lcd.print(title);  // berlin repeats station name
  if(station !=2) splitText(title);
}

void audio_info(const char *info)  // for information purposes
{
  String chk = String(info);  // convert char array to string
  if(chk.startsWith("Connect")) Serial.println(info);
//  Serial.println(info);
}
