#include <Arduino.h>
#include "main.h"
#include "common_point.h"


// Define task handles
TaskHandle_t generate_handle;
TaskHandle_t serial_handle;
TaskHandle_t flash_handle;

//Initialize common buffers
static CoordBuff coordBufs[LAYER_SIZE]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
static BitLayerBuff bitBufs[LAYER_SIZE];     

//Initialize arrays for Serial Function
point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE];
int points_count[LAYER_SIZE];


void serial_task(void *pvParameters) {
  const TickType_t xDelay = 1;  // 1 clock cycle (placeholder)
  uint8_t currLayer = 0;

  //const uint8_t numChars = strlen("173824481236384712536823764837285176");
  //char serialBuffer[numChars + 1];   // an array to store the received data


  const char *numString = "173824481236384712536823764837285176";
  char serialBuffer[strlen(numString) + 1];   // Add 1 for the null terminator

  strcpy(serialBuffer, numString);  // Copy the numString into serialBuffer

#if TEST
  //const int numChars = 213; // 'xyz'*512
  //char serialBuffer[numChars];   // an array to store the received data
  //char input[] = "173824481236384712536823764837285176143647326754831257248176832641285782473265814386427187";
  //strcpy(serialBuffer, input);
  //bool readyToProcess = true;
  #endif
  bool readyToProcess = false;
#if LABVIEW  
  //Initializers for serial read
  uint8_t ndx = 0;
  char endMarker = '\n';
  bool readyToProcess = false;
  #endif

  while (1) {

    #if LOGGING
      Serial.println("Enters serial task");
    #endif
    #if SERIAL_TASK_EN
      // read from serial buffer, convert one coord string to int x, y, z at a time
      // store the int x, y, z in a temperary buffer
      // move this x,y,z into the coordBufs
      #if !LABVIEW
        if (!readyToProcess) {
          process_serial_string(serialBuffer, layer_points, points_count);
          readyToProcess = true;
        }
      #endif
      if (readyToProcess) { //priortize processing? this will be blocked if no mutex is available tho and might overflow the buffer 
        // DO PREPROCESSING TO FILL COORDINATE BUFFERS
        
        //xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); 
        // // 2.2. Fill the coordinate buffers (Critical Section - try as short as possible)
        // // 2.3. Return the mutex
        //xSemaphoreGive(coordBufs[currLayer].mutex);
        
        Serial.printf("Enters serial task, point count for current layer: %d\n", points_count[currLayer]);

        if (currLayer >= LAYER_SIZE - 1) {
          currLayer = 0;
          readyToProcess = false;
        } else {
          currLayer++;
        }
        
      }
      #if LABVIEW
      else if (Serial.available()){ // non-blocking
        char ch = Serial.read();
        if (ch != endMarker) {
          serialBuffer[ndx] = ch;
          ndx++;
          if (ndx >= numChars) { // ring buffer loop-around mechanism
              ndx = 0;
          }
        } else { 
          process_serial_string(serialBuffer, layer_points, points_count);
          readyToProcess = true;
          ndx = 0;
        }
      }
      #endif
    #endif
  vTaskDelay(xDelay); // Delay 1 cycle
  }
}

void generate_task(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(100);
  uint8_t currLayer = 0;
  bitset_t tempBuffer[ROWS]; // Temporary storage for buffer

  while (1) {
    #if LOGGING 
      Serial.println("Enters generate task");
    #endif
    #if GENERATE_TASK_EN
      // Take mutexes (blocked if any is unavailable)
      // xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // change max delay to skip?
      // // Convert coordinates to bitmaps (generate.c)
      // // maybe a temporary storage here can help split up the critical section into two?
      // xSemaphoreGive(coordBufs[currLayer].mutex);

      // xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
      // // copy the data to bitbuf
      // xSemaphoreGive(bitBufs[currLayer].mutex);
      if (currLayer >= LAYER_SIZE-1) {
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
      if (currLayer >= LAYER_SIZE-1) {
        currLayer = 0;
      } else {
        currLayer++;
      }
    #endif
  }
}

void setup() {
  Serial.begin(9600); 
  // Initializes mutexes (buffer values automatically set to zero when declared as static variables)
  for (int i = 0; i < LAYER_SIZE; i++) {
    coordBufs[i].mutex = xSemaphoreCreateMutex();
    bitBufs[i].mutex = xSemaphoreCreateMutex();
  }
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    generate_task,     /* Task function. */
                    "generate",        /* name of task. */
                    10000,        /* Stack size of task */
                    NULL,              /* parameter of the task */
                    2, /* priority of the task */
                    &generate_handle,  /* Task handle to keep track of created task */
                    0);                /* pin task to core 0 */                  
  xTaskCreatePinnedToCore(
                    serial_task,      /* Task function. */
                    "serial",         /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,             /* parameter of the task */
                    2,  /* priority of the task */
                    &serial_handle,   /* Task handle to keep track of created task */
                    0);               /* pin task to core 0 */                  
  xTaskCreatePinnedToCore(
                    flash_task,      /* Task function. */
                    "flash",         /* name of task. */
                    10000,      /* Stack size of task */
                    NULL,            /* parameter of the task */
                    1,  /* priority of the task */
                    &flash_handle,   /* Task handle to keep track of created task */
                    1);              /* pin task to core 1 */   

}

void loop() {
  
}



