/*******************************************************************************
 * Sketch name: Elapsed time between sleep mode and reset
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <sys/time.h>           // system time library
RTC_DATA_ATTR time_t rebootTime;      // variable in RTC memory
struct timeval timeData;        // time structure
int interval, sleepSec;
unsigned long uSec = 1000000;

void setup()
{
  Serial.begin(115200);
  gettimeofday(&timeData, NULL);      // time at reboot
  interval = timeData.tv_sec - rebootTime;  // interval (sec) since reboot
  Serial.printf("slept for %d s \n", interval);
  rebootTime = timeData.tv_sec;     // update reboot time
  sleepSec = random (1, 10);
  Serial.printf("sleep for %d s \n", sleepSec);
  esp_deep_sleep(sleepSec * uSec);      // sleep for sleepSec seconds
}

void loop()
{}                // nothing in loop function
