/*******************************************************************************
 * Sketch name: Memory storage properties
 * Description: Different methods to measure properties of heap memory
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

void setup()
{
  Serial.begin(115200);          // Serial Monitor baud rate
  Serial.println("\n         total  free   alloc large");
  multi_heap_info_t info;       // information structure
  heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);  // heap memory
  Serial.printf("DEFAULT  %d %d %d %d \n",
  heap_caps_get_total_size(MALLOC_CAP_DEFAULT),   // total heap size
  info.total_free_bytes,          // free heap
  info.total_allocated_bytes,         // allocated heap
  info.largest_free_block);            // largest unallocated block

  heap_caps_get_info(&info, MALLOC_CAP_INTERNAL); // internal memory
  Serial.printf("INTERNAL %d %d %d %d \n",
  heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
  info.total_free_bytes,
  info.total_allocated_bytes,
  info.largest_free_block);

  Serial.printf("heap     %d %d %d %d \n",      // "heap" characteristics
  ESP.getHeapSize(),ESP.getFreeHeap(),
  ESP.getHeapSize()-ESP.getFreeHeap(), ESP.getMaxAllocHeap());

  heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);  // equivalent instructions
  Serial.println("\nheap diff instructions same results");
  Serial.printf("free heap     %d %d %d \n",
  info.total_free_bytes,          // available heap
  heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
  esp_get_free_heap_size());
  Serial.printf("largest block %d %d \n",
  info.largest_free_block,             // largest unallocated block
  heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
}

void loop()
{}
