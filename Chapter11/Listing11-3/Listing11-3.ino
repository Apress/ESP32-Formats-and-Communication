/*******************************************************************************
 * Sketch name: ESP32-CAM module configuration
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

camera_config_t config;        // camera configuration parameters
void configCamera()
{
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_xclk = 0;            // external clock
  config.pin_pwdn = 32;           // power down
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_pclk = 22;           // pixel clock
  config.pin_vsync = 25;          // vertical synchronisation
  config.pin_href = 23;           // horizontal reference
  config.pin_sscb_sda = 26;         // I2C data
  config.pin_sscb_scl = 27;         // I2C clock
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;       // clock speed of 20MHz
  config.pixel_format = PIXFORMAT_JPEG;     // JPEG file format
  config.frame_size = FRAMESIZE_QVGA;     // 320240 pixels
  config.jpeg_quality = 10;         // image quality index
  config.fb_count = 1;            // frame buffer count
  esp_err_t err = esp_camera_init(&config);   // initialize camera
  if (err != ESP_OK)
  {
    Serial.print("Camera initialise failed with error");
    Serial.println(err);
    return;
  }
}
