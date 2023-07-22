/*******************************************************************************
 * Sketch name: Fast Fourier Transform
 * Description: Derive and display signal FFT components
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

void FFTdisplay()
{
  int FFTn = FFTbufferLen;          // FFTbufferLen defined
  int16_t FFTbuffer[FFTbufferLen] = {0};      //    in main sketch
  size_t bits = 0;
  double data = 0;
  int FFTdata[128], plotData[24], temp;     // FFT output values
  fft_config_t * FFT = fft_init(FFTn, FFT_REAL, FFT_FORWARD, NULL, NULL);
  i2s_read(I2S_NUM_0, &FFTbuffer, sizeof(FFTbuffer), &bits, // read I2S buffer
(100 / portTICK_RATE_MS));  //    every 100ms
  for (int i=0; i<FFT->size; i++) FFT->input[i] = // constrain & map buffer
      map(FFTbuffer[i], INT16_MIN, INT16_MAX, -2000, 2000);
  fft_execute(FFT);           // FFT analysis
  for (int i=1; i<FFT->size/4; i++)
  {               // magnitude = real2 + imaginary2
    data = sqrt(pow(FFT->output[2*i],2) + pow(FFT->output[2*i+1],2));
    if(i - 1 < 128)
    {
      data = constrain(data, 0, 2000);      // constrain and map
      FFTdata[128-i] = map(data, 0, 2000, 0, 255);  //    FFT magnitudes
    }
  }
  fft_destroy(FFT);           // free-up memory space
  for (int i=0; i<24; i++)
  {
    temp = 0;             // 
    for (int j=0; j<5; j++) temp = temp + FFTdata[i * 5 + j];
    temp = round(temp/5.0);           // average of 5 values
    plotData[i] = map(temp, 0, 255, 0, 16);   // height of column = 16
  }
  M5.Lcd.fillRect(0,120,320,120, BLACK);      // clear half LCD screen
  for (int i=0; i<24; i++)
  {
    x[0][i] = plotData[i];          // FFT magnitude
    x[1][i] = i;              // FFT column
  }
  sort();     // sort plotData in descending order, retain column values
  for (int i=0; i<24; i++)        // FFT display with
  {               //     24 columns, 16 blocks high
    int k = 23-i;           // lowest frequency displayed first
    for (int j=0; j<16; j++)        // columns colour coded
    {               //    red, orange or yellow
      if((k == x[1][0] || k == x[1][1] || k == x[1][2]) && j < plotData[k])
                   M5.Lcd.fillRect(i * 12, 234 - j * 6 - 5, 5, 5, RED);
 else if((k == x[1][3] || k == x[1][4] || k == x[1][5]) && j < plotData[k])
                   M5.Lcd.fillRect(i * 12, 234 - j * 6 - 5, 5, 5, ORANGE);
 else if(j <  plotData[k])
                   M5.Lcd.fillRect(i * 12, 234 - j * 6 - 5, 5, 5, YELLOW);
 else if(j == plotData[k])
                   M5.Lcd.fillRect(i * 12, 234 - j * 6 - 5, 5, 5, GREEN);
    }               // column top in green
  }
  if(millis() - last > 1000)        // update main FFT frequency
  {
    last = millis();
    if(x[1][0] > 0)         // if audio detected
    {
      float freq = freqCalc();      // function to calculate frequency
      String str = String(freq, 0) + "Hz";
      M5.Lcd.fillRect(200, 0, 120, 30, BLACK);
      M5.Lcd.drawString(str, 200, 0, 4);    // display frequency
    }
    else M5.Lcd.fillRect(200, 0, 120, 30, BLACK);
  }
}
