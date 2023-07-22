/*******************************************************************************
 * Sketch name: Stream_handler
 * Description: Function for Listing 11-7
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#define Boundary "123456789000000000000987654321"
static const char* ContentType =
     "multipart/x-mixed-replace;boundary=" Boundary;
static const char* StreamBound = "\r\n--" Boundary  "\r\n";
static const char* StreamContent =
     "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static esp_err_t stream_handler(httpd_req_t *req)
{
  camera_fb_t * frame = NULL;      // associate frame with esp_camera
  uint8_t * jpgBuffer = NULL;       // JPEG buffer
  size_t jpgLength = 0;         // length of JPEG buffer
  char * part_buf[64];
  esp_err_t res = ESP_OK;       // error status
  res = httpd_resp_set_type(req, ContentType);
  if (res != ESP_OK) return res;
  while (true)
  {
   frame = esp_camera_fb_get();     // get camera image
   if (!frame) {Serial.println("Camera capture failed"); res = ESP_FAIL;}
// set JPEG buffer length
   else {jpgLength = frame->len; jpgBuffer = frame->buf;}
   if (res == ESP_OK)         // no error, stream image
   {
    size_t hlen = snprintf((char *)part_buf, 64, StreamContent, jpgLength);
    res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
   }
   if (res == ESP_OK)
res = httpd_resp_send_chunk(req, (const char *)jpgBuffer, jpgLength);
    if (res == ESP_OK)
res = httpd_resp_send_chunk(req, StreamBound, strlen (StreamBound));
    if (frame)            // return frame buffer for reuse
      {esp_camera_fb_return(frame); frame = NULL; jpgBuffer = NULL;}
    else if (jpgBuffer) {free(jpgBuffer); jpgBuffer = NULL;}
    if (res != ESP_OK) break;
  }
  return res;
}
