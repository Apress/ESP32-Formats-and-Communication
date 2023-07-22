/*******************************************************************************
 * Sketch name: Device handler with esp_http_server
 * Description: Function for Listing 11-15
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

static esp_err_t device_handler(httpd_req_t *req)
{
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char value[32] = {0,};
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1)
  {
    buf = (char*)malloc(buf_len);
    if (!buf) {httpd_resp_send_500(req); return ESP_FAIL;}
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
    {
   if(httpd_query_key_value(buf,"device",variable,sizeof(variable))==ESP_OK
   && httpd_query_key_value(buf,"level",value,sizeof(value))==ESP_OK) {}
   else {free(buf); httpd_resp_send_404(req); return ESP_FAIL;}
   }
   else {free(buf); httpd_resp_send_404(req); return ESP_FAIL;}
   free(buf);
  }
  else {httpd_resp_send_404(req); return ESP_FAIL;}
  
  int x = atoi(value);          // convert value char to integer
  if(!strcmp(variable, "light"))      // HTTP request with "light"
  {
    digitalWrite(lightPin, x);      // x = 0 or 1
    Serial.printf("light %d \n", x);
  }
  else if(!strcmp(variable, "servo"))   // HTTP request with "servo"
  {               // x = 20 to 160
    pulse = round(pow(2,resol)*(2.5 + x/18.0)/100.0);
    ledcWrite(channel, pulse);
    Serial.printf("servo %d pulse %d \n", x, pulse);
  }
  else if(!strcmp(variable, "button"))    // HTTP request with "button"
  {
    String str = value;         // convert value char to string
    Serial.printf("button %s \n", str);
  }
  return httpd_resp_send(req, NULL, 0);
}
