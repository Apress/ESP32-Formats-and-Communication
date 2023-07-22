/*******************************************************************************
 * Sketch name: Digital audio display
 * Description: Display audio signal on M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>          // include M5Core2 and
#include <driver/i2s.h>         //    ESP32 I2S libraries
const int bufferLen = 1024;       // DMA buffer size (samples)
int N = 1, yData[320], yBase = 120;
int minY = 0, maxY = 2000;        // min and max values M5GO
//int minY = -600, maxY = 600;       // min and max values M5Stack

void setup()
{
  M5.begin();
  M5.Lcd.fillScreen(BLACK);       // initialise display
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.drawString("PDM mic", 50, 0, 4);
  for (int i=0; i<320; i++) yData[i] = yBase;
  I2Sconfig();            // configure I2S
  pinConfig();            //    and GPIO pins
}

void I2Sconfig()            // function to configure I2S
{
  i2s_config_t i2s_config = {       // receive and PDM modes
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 44100,         // sample frequency
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // 16-bit sampling
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // mono channel sampling
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,           // DMA buffers
    .dma_buf_len = bufferLen          // DMA buffer length
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
}

void pinConfig()            // function to configure I2S pins
{
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_PIN_NO_CHANGE,    // bit clock frequency
    .ws_io_num = 0,         // word select (left /right) clock
    .data_out_num = I2S_PIN_NO_CHANGE,    // data output
    .data_in_num = 34         // data input
  };
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop()
{
  wave();             // call wave function
}

void wave()       // function to read DMA buffer and display waveform
{
  size_t bits = 0;
  int16_t buffer[bufferLen] = {0};      // define and read I2S data buffer
  i2s_read(I2S_NUM_0, &buffer, sizeof(buffer), &bits, portMAX_DELAY);
  int bytes = bits / 8;         // convert bits to bytes
  if(bytes > 0)
  {
    for (int i=0; i<bytes; i=i+N)
    {
      M5.Lcd.drawLine(0, yData[0], 1, yData[1], BLACK); // overwrite value
      for (int j=1; j<320; j++) yData[j-1] = yData[j];     // shift one position
      int temp = constrain(buffer[i], minY, maxY);   // constrain buffer values
   Serial.println(buffer[i]);
      yData[319] = map(temp, minY, maxY, 30, 240);      // map to LCD height
      for (int j=1; j<319; j++)
      {
        M5.Lcd.drawLine(j, yData[j-1], j+1, yData[j], BLACK); // overwrite and 
        M5.Lcd.drawLine(j, yData[j],   j+1, yData[j+1], RED); // draw new line
      }
    }
  }
  if (M5.BtnA.isPressed())        // press button A to increase lag
  {
    N++;
    newValue();           // call function to display lag
  }
  if (M5.BtnB.isPressed())        // press button B to decrease lag
  {
    N--;
    newValue();
  }
  if (M5.BtnC.isPressed())        // press button C to set lag to one
  {
    N = 1;
    newValue();
  }
  M5.update();
}

void newValue()         // function to update displayed lag
{
  if(N > 60 || N < 1) N = 1;
  M5.Lcd.fillRect(200, 0, 120, 30, BLACK);
  String str = "lag = " + String(N);
  M5.Lcd.drawString(str, 200, 0, 4);
}
