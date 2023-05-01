/* freeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* library includes */
#include <Arduino.h>
#include <stdint.h>

#include "main.h"

// Define task handles
TaskHandle_t generate_handle;
TaskHandle_t serial_handle;
TaskHandle_t flash_handle;

static CoordBuff coordBufs[LAYER_SIZE];
static BitLayerBuff bitBufs[LAYER_SIZE];

void setup(){
  #if LOGGING 
    Serial.begin(9600);
  #endif
  // Initializes FreeRTOS
  #if LOGGING 
    Serial.println("Starting FreeRTOS scheduler");
  #endif
  // Initializes mutexes (buffer values automatically set to zero when declared as static variables)
  for (int i = 0; i < LAYER_SIZE; i++) {
    coordBufs[i].mutex = xSemaphoreCreateMutex();
    bitBufs[i].mutex = xSemaphoreCreateMutex();
  }
  // Create tasks
  xTaskCreateStaticPinnedToCore(
                    generate_task,     /* Task function. */
                    "generate",        /* name of task. */
                    STACK_SIZE,        /* Stack size of task */
                    NULL,              /* parameter of the task */
                    GENERATE_PRIORITY, /* priority of the task */
                    &generate_handle,  /* Task handle to keep track of created task */
                    0);                /* pin task to core 0 */                  
  xTaskCreateStaticPinnedToCore(
                    serial_task,      /* Task function. */
                    "serial",         /* name of task. */
                    STACK_SIZE,       /* Stack size of task */
                    NULL,             /* parameter of the task */
                    SERIAL_PRIORITY,  /* priority of the task */
                    &serial_handle,   /* Task handle to keep track of created task */
                    0);               /* pin task to core 0 */                  
  xTaskCreatePinnedToCore(
                    flash_task,      /* Task function. */
                    "flash",         /* name of task. */
                    STACK_SIZE,      /* Stack size of task */
                    NULL,            /* parameter of the task */
                    FLASH_PRIORITY,  /* priority of the task */
                    &flash_handle,   /* Task handle to keep track of created task */
                    1);              /* pin task to core 1 */                  

  // there is an implicit idle task created by the scheduler to run when all are blocked

  // Start scheduler
  vTaskStartScheduler();

  // error checking
  #if LOGGING 
    Serial.println("Scheduler ended");
  #endif
}

void loop(){
  // This function should never be called as FreeRTOS takes over control
  #if LOGGING 
    Serial.println("SYSTEM FAULT");
  #endif
  vTaskEndScheduler(); // to stop the processes
  while (1) {}
}

void serial_task(void *pvParameters) {
  const TickType_t xDelay = 1;  // 1 clock cycle (placeholder)

  const uint8_t numChars = 3720; // 'x y z\n'*512
  char serialBuffer[numChars];   // an array to store the received data
  
  uint8_t ndx = 0;
  char endMarker = '\n';
  bool readyToProcess = false; //what are those three lines?

  uint8_t currLayer = 0;
  while (1) {
    #if LOGGING 
      Serial.println("Enters serial task");
    #endif
    #if SERIAL_TASK_EN
      // read from serial buffer, convert one coord string to int x, y, z at a time
      // store the int x, y, z in a temperary buffer
      // move this x,y,z into the coordBufs

      if (readyToProcess) { //priortize processing? this will be blocked if no mutex is available tho and might overflow the buffer 
        // DO PREPROCESSING TO FILL COORDINATE BUFFERS
        // 1. Convert strings to ints
        // 2.1. Take the mutex (blocked if busy, change max delay to skip?)
        xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); 
        // 2.2. Fill the coordinate buffers (Critical Section - try as short as possible)
        // 2.3. Return the mutex
        xSemaphoreGive(coordBufs[currLayer].mutex);
        if (currLayer >= LAYER_SIZE) {
          currLayer = 0;
          readyToProcess = false;
        } else {
          currLayer++;
        }
      } else if (Serial.available()){ // non-blocking
        ch = Serial.read();
        if (ch != endMarker) {
          serialBuffer[ndx] = ch;
          ndx++;
          if (ndx >= numChars) { // ring buffer loop-around mechanism
              ndx = 0;
          }
        } else { 
          readyToProcess = true;
          ndx = 0;
        }
      }
    #endif
    vTaskDelay(xDelay); // Delay 1 cycle
  }
}

void generate_task(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(100);

  uint8_t currLayer = 0;
  while (1) {
    #if LOGGING 
      Serial.println("Enters generate task");
    #endif
    #if GENERATE_TASK_EN
      // Take mutexes (blocked if any is unavailable)
      xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // change max delay to skip?
      // Convert coordinates to bitmaps (generate.c)
      // maybe a temporary storage here can help split up the critical section into two?
      xSemaphoreGive(coordBufs[currLayer].mutex);

      xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
      // copy the data to bitbuf
      xSemaphoreGive(bitBufs[currLayer].mutex);
      if (currLayer >= LAYER_SIZE) {
        currLayer = 0;
      } else {
        currLayer++;
      }
    #endif
    vTaskDelay(xDelay); // Delay 100 milliseconds
    // note that vTaskDelay is non-blocking (enable context switching), but the arduino delay() is blocking (i.e. the CPU will be stuck here)
  }
}

void flash_task(void *pvParameters) {
  // to run at a certain frequency
  const TickType_t xFrequency = 10;
  TickType_t xLastWakeTime;
  BaseType_t xWasDelayed;
  xLastWakeTime = xTaskGetTickCount();  // get initial time t0
  uint8_t currLayer = 0;
  while (1) {
    #if LOGGING 
      Serial.println("Enters flash task");
    #endif
    // Wait for the clock to tick
    xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
    #if FLASH_TASK_EN 
      // Takes the mutex
      xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
      // copy the data to local to avoid blocking the access for generate to use bitBufs?
      xSemaphoreGive(bitBufs[currLayer].mutex);
      // Returns the mutex
      if (currLayer >= LAYER_SIZE) {
        currLayer = 0;
      } else {
        currLayer++;
      }
    #endif
  }
}

/*
Alternative structure to peek mutex and loop through to skip blocked access to buffers:
if(xQueuePeek((xQueueHandle)THE_mutex,(void *)NULL,(portTickType)NULL) != pdTRUE){  
  // the mutex is taken  
}else{
  // the mutex is free  
}  
*/