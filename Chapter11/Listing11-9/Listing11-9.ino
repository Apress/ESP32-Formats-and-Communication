/*******************************************************************************
 * Sketch name: Page_handler
 * Description: Function for Listing 11-7
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

static esp_err_t page_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)page, strlen(page));
}
