#include "FreeRTOS.h"
#include "task.h"

// Define task handles
TaskHandle_t generator_handle;
TaskHandle_t main_handle;
TaskHandle_t read_handle;

// Defining task functions
void generator_task(void *pvParameters);
void main_task(void *pvParameters);
void read_task(void *pvParameters);

void setup(){
  // Initializing FreeRTOS
  // ...

  // Create tasks
  xTaskCreate(generator_task, "Generator", 1000, NULL, 3, &generator_handle);
  xTaskCreate(main_task, "Main", 1000, NULL, 2, &main_handle);
  xTaskCreate(read_task, "Read", 1000, NULL, 1, &read_handle);

  // Start scheduler
  vTaskStartScheduler();
}

void loop(){
  // This function should never be called as FreeRTOS takes over control
}

void generator_task(void *pvParameters) {
  // This is the code from your generator.c file
  // ...

  while (1) {
    // ...
    // Generate buffers and send them to main task
    // ...
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay 100 milliseconds
  }
}

void main_task(void *pvParameters) {
  // This is the code from your main.c file
  // ...

  while (1) {
    // ...
    // Receive buffers from generator task and flash LEDs
    // ...
    vTaskDelay(pdMS_TO_TICKS(50)); // Delay 50 milliseconds
  }
}

void read_task(void *pvParameters) {
  // This is the code from your read.c file
  // ...

  while (1) {
    // ...
    // Read coordinates from Labview and store them in arrays
    // ...
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay 10 milliseconds
  }
}
