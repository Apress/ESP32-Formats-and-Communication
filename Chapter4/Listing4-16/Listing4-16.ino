/*******************************************************************************
 * Sketch name: Source OpenWeatherMap API data
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appWeather()        // function to display local weather information
{
  tft->fillScreen(TFT_BLACK);
  tft->drawBitmap(0, 0, weatherImage, 32, 32, TFT_WHITE); // draw icon
  tft->setCursor(50, 10);
  tft->print("connecting");
  int wifiOK = WiFiconnect();       // defined on appWwwTime tab
  if(wifiOK == 0) return;       // no Wi-Fi connection
  String URL = "http://api.openweathermap.org/data/2.5/weather?";
  URL = URL + "id=2650225&units=metric&appid="; // URL with city code in bold
  URL = URL + APIkey;         // include APIkey in URL
  String data = http(URL);        // source OpenWeatherMap data
  DynamicJsonDocument doc(1024);      // JSON document
  StaticJsonDocument<200> filter;     // filter for JSON data
  filter["weather"][0]["main"] = 1;
  filter["main"]["feels_like"] = 1;
  filter["main"]["temp_max"] = 1;     // define subset of JSON data
  filter["main"]["humidity"] = 1;
  filter["wind"]["speed"] = 1;
  filter["clouds"]["all"] = 1;
  filter["name"] = 1;         // deserialise filtered JSON data
  deserializeJson(doc, data, DeserializationOption::Filter(filter));
  String main = doc["weather"][0]["main"];
  float feels = doc["main"]["feels_like"];
  float temp = doc["main"]["temp_max"];   // update variables
  float humidity = doc["main"]["humidity"];
  float wind = doc["wind"]["speed"];
  float clouds = doc["clouds"]["all"];
  String city = doc["name"];

  tft->fillScreen(TFT_BLACK);
  tft->drawBitmap(0, 0, weatherImage, 32, 32, TFT_WHITE);
  tft->setTextColor(TFT_YELLOW);
  tft->setCursor(50, 10);
  tft->printf("%s \n\n ", city);        // display variables
  tft->setTextColor(TFT_GREEN);
  tft->printf("Weather %s \n\n ", main);
  tft->printf("Feels like %.1fC \n\n ", feels);
  tft->printf("Temp max %.1fC \n\n ", temp);
  tft->printf("Humidity %.0f%% \n\n ", humidity);
  float kmh = 3.6 * wind;       // convert m/s to km/h
  tft->printf("Wind %.1fkm/h \n\n ", kmh);
  tft->printf("Clouds %.0f%% \n", clouds);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  int16_t x, y;
  while(!ttgo->getTouch(x, y)) {}     // do nothing waiting for touch
  while (ttgo->getTouch(x, y)) {}     //      then wait for touch release
}

String http(String URL)   // function to source OpenWeatherMap data from URL
{
  HTTPClient http;          // associate http with library
  http.begin(URL);          // OpenWeatherMap URL
  http.GET();           // HTTP request
  String data = http.getString();
  http.end();
  return data;            // return data to function
}
