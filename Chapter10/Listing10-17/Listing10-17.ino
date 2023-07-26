/*******************************************************************************
 * Sketch name: Heap size with large array
 * Description: Demonstrate impact on heap of array and images
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
TFT_eSprite img = TFT_eSprite(&tft);    // Sprite image pointer to img
int A[500];             // integer array

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  Serial.println();
  A[0] = 1;
  Serial.printf("Size of A %d bytes \n", sizeof(A));  // array size in bytes
  img.setColorDepth(8);           // Sprite colour depth
  img.createSprite(240, 240);         // Sprite dimensions
  multi_heap_info_t info;         // information structure
  heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);
  Serial.printf("total %d alloc %d free %d large %d \n",
  heap_caps_get_total_size(MALLOC_CAP_DEFAULT),   // total heap size in bytes
  info.total_allocated_bytes,         // allocated heap
  info.total_free_bytes,          // available heap
  info.largest_free_block);         // largest unallocated heap
}

void loop()               // nothing in loop function
{}
