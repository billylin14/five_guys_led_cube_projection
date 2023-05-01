#include <Arduino.h>
#include "main.h"
#include "common_point.h"
#include "stdio.h"
#include "esp_task_wdt.h"


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
  const TickType_t xDelay = pdMS_TO_TICKS(100);  // 100 clock cycle (placeholder)
  uint8_t currLayer = 0;

  #if !LABVIEW
    const char *numString = "173824481236384712536823764837285176";
    // 173* 824* 481* 236* 384* 712* 536* 823* 764* 837* 285* 176* // Process_to_serial works
    char serialBuffer[strlen(numString) + 1];   // Add 1 for the null terminator
    strcpy(serialBuffer, numString);  // Copy the numString into serialBuffer
  #else
    //Initializers for serial read
    unsigned int numChars = 512;
    char serialBuffer[numChars+1];
    unsigned int ndx = 0;
    char endMarker = '\n';
  #endif
  
  bool readyToProcess = false;

  while (1) {
    #if LOGGING
      Serial.println("Enters serial task");
      Serial.printf("\n");
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
        
        // set MSB to 1
        // 2.1. Take the mutex (blocked if busy, change max delay to skip?)
        xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY);

        #if LOGGING
          Serial.printf("Current Layer: %d\n", currLayer);  
          Serial.printf("Enters serial task, point count for current layer: %d\n", points_count[currLayer]);
        #endif
        // 2.2. Fill the coordinate buffers (Critical Section - try as short as possible)
        for (int i = 0; i < points_count[currLayer]; i++) { //The number of points in the current layer is given here
          coordBufs[currLayer].points[i].x = layer_points[currLayer][i].x;
          /*This line assigns the x-coordinate value of the i-th point in the layer_points array for the current layer 
          to the x-coordinate value of the i-th point in the coordBufs array for the same layer*/
          coordBufs[currLayer].points[i].y = layer_points[currLayer][i].y;
        }
        coordBufs[currLayer].point_count = points_count[currLayer]; //store point_count locally

        /*assigns the total number of points in the current layer 
        to the point_count "field (the struct)" of the CoordBuff struct for the same layer.*/

        xSemaphoreGive(coordBufs[currLayer].mutex);

        #if LOGGING
          // Print layer points and coordbuffs
          for (int i = 0; i < points_count[currLayer]; i++) {
            Serial.printf("Current Layer_Points: (%d, %d)\n", layer_points[currLayer][i].x, layer_points[currLayer][i].y);
          }
          Serial.printf("\n");
          for (int i = 0; i < points_count[currLayer]; i++) {
            Serial.printf("From CoordBuffs(%d, %d, %d)\n", coordBufs[currLayer].points[i].x, coordBufs[currLayer].points[i].y, currLayer);
          }
          Serial.printf("\n");
          Serial.printf("\n");
        #endif
        
        if (currLayer >= LAYER_SIZE - 1) {
          currLayer = 0;
          #if LOGGING
            Serial.printf("Current Layer: %d\n", currLayer);  
            Serial.printf("\n");
          #endif
          readyToProcess = false;
        } else {
          currLayer++;
          #if LOGGING
            Serial.printf("Current Layer: %d\n", currLayer); 
            Serial.printf("\n");
          #endif
        }
        
      }
      #if LABVIEW
          while (Serial.available() > 0) { // non-blocking
            char ch = Serial.read();
<<<<<<< Updated upstream
            Serial.print("Received: ");
            Serial.println(ch);
=======
            #if LABVIEW
              Serial.print(ch);
            #endif
>>>>>>> Stashed changes
            if (ch != endMarker) {
              serialBuffer[ndx] = ch;
              ndx++;
              if (ndx >= numChars) { // ring buffer loop-around mechanism
                ndx = 0;
              }
<<<<<<< Updated upstream
              Serial.print("Buffer index: ");
              Serial.println(ndx);
              Serial.print("Buffer content: ");
              Serial.println(serialBuffer);
            } else {
              // Serial.println("Billy is smart");
=======
            } else {
>>>>>>> Stashed changes
              serialBuffer[ndx] = '\0';
              // Serial.print(serialBuffer);
              process_serial_string(serialBuffer, layer_points, points_count);
              // readyToProcess = true;
              ndx = 0;
            }
          }
        #endif
    #endif
  vTaskDelay(xDelay); // Delay 100 cycle
  }
}

void generate_task(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(50); //1/2 the delay time of serial
  uint8_t currLayer = 0;
  bitset_t tempBuffer[ROW_SIZE]; // Temporary storage for buffer

  while (1) {
    #if LOGGING 
      Serial.println("Enters generate task");
    #endif
    #if GENERATE_TASK_EN
      // Take mutexes (blocked if any is unavailable)
      xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // change max delay to skip?
      // // Convert coordinates to bitmaps (generate.c)
      fill_temp_buffer_with_coords(tempBuffer, &coordBufs[currLayer]); //check if & works
      xSemaphoreGive(coordBufs[currLayer].mutex);
      //print current buffer layer
      // Serial.printf("Generate function ran %d layer \n", currLayer);
      // moves temp buffer to bitBufs for flashing data
      //Print Statements
      xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
      for (int i = 0; i < ROW_SIZE; i++) {
          bitBufs[currLayer].buff[i] = tempBuffer[i];
          #if LOGGING
            for (int j=0; j < COL_SIZE; j++){
              Serial.printf("%d", GetBit(bitBufs[currLayer].buff[i],j));
            }
          Serial.printf("\n");
          Serial.printf("\n");
          #endif
      }

      xSemaphoreGive(bitBufs[currLayer].mutex);

      //update layer for every loop
      if (currLayer >= LAYER_SIZE-1) {
        currLayer = 0;
        #if LOGGING
          Serial.printf("Current Layer is: %d", currLayer);
          Serial.printf("\n");
        #endif

      } else {
        currLayer++;
        #if LOGGING
          Serial.printf("Current Layer is: %d", currLayer);
          Serial.printf("\n");
        #endif
      }


    #endif
    vTaskDelay(xDelay); // Delay 100 milliseconds
    // note that vTaskDelay is non-blocking (enable context switching), but the arduino delay() is blocking (i.e. the CPU will be stuck here)
  }
}


void flash_task(void *pvParameters) {
  // to run at a certain frequency
  const TickType_t xFrequency = 30 / portTICK_PERIOD_MS;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();  // get initial time t0

  uint8_t currLayer = 0;

  while (1) {

    #if FLASH_TASK_EN 
      #if LOGGING 
        Serial.println("Enters flash task");
        Serial.printf("Current Layer is: %d", currLayer);
        Serial.printf("\n");
      #endif

      // Takes the mutex
      xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
      //upload a layer of bits to shift register
      #if LOGGING
        Serial.printf("Printing Layer Write: \n\n");
      #endif
      layer_write(&bitBufs[currLayer]);
      xSemaphoreGive(bitBufs[currLayer].mutex);

      #if LOGGING
        Serial.printf("\n");
      #endif
      // Returns the mutex
      //Tell 8 bit shift register to light new layer ********** //
      digitalWrite(layer_clk,0);                                //
      //check if flag is past 8 layers--> reset layer count     //
      if(currLayer == 0) {                                     //
        digitalWrite(Layer, 1);                                 //
      } else {                                                  //
        digitalWrite(Layer, 0);                                 //
      }                                                         //
      digitalWrite(layer_clk, 1);                               //
      //********************************************************//
      //update layer flag for upcoming layer
      currLayer++;

      //reset counter once it reaches final layer
      if(currLayer >= LAYER_SIZE){
        currLayer = 0;
      }
    #endif
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}

void setup() {
  Serial.begin(115200);
  esp_task_wdt_init(10, false); // Increase the timeout to 10 seconds
  //set pinModes
  pinMode(Layer, OUTPUT);
  pinMode(Row1, OUTPUT);
  pinMode(Row2, OUTPUT);
  pinMode(Row3, OUTPUT);
  pinMode(Row4, OUTPUT);
  pinMode(Row5, OUTPUT);
  pinMode(Row6, OUTPUT);
  pinMode(Row7, OUTPUT);
  pinMode(Row8, OUTPUT);
  pinMode(row_clk, OUTPUT);
  pinMode(layer_clk, OUTPUT); 
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
                    1, /* priority of the task */
                    &generate_handle,  /* Task handle to keep track of created task */
                    1);                /* pin task to core 0 */                  
  xTaskCreatePinnedToCore(
                    serial_task,      /* Task function. */
                    "serial",         /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,             /* parameter of the task */
                    2,  /* priority of the task */
                    &serial_handle,   /* Task handle to keep track of created task */
                    1);               /* pin task to core 0 */                  
  xTaskCreatePinnedToCore(
                    flash_task,      /* Task function. */
                    "flash",         /* name of task. */
                    10000,      /* Stack size of task */
                    NULL,            /* parameter of the task */
                    1,  /* priority of the task */
                    &flash_handle,   /* Task handle to keep track of created task */
                    0);              /* pin task to core 1 */
                    //Change to run every __ms   

}

void loop() {
  
}



