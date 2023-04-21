/*******************************************************************************
 * Sketch name: ADC voltage measurement
 * Description: Measure voltage on GPIO pin with different methods
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

#include <esp_adc_cal.h>
int ADCpin = 37;
int count = 0, raw, analog, mVraw, mVanalog;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  raw = adc1_get_raw(ADC1_CHANNEL_6);  // GPIO 37 or A1
  mVraw = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
  analog = analogRead(ADCpin);
  mVanalog = analogReadMilliVolts(ADCpin);
  count++;
  Serial.printf("%d %d %d %d %d \n", count, raw, analog, mVraw, mVanalog);
  delay(100);
}
