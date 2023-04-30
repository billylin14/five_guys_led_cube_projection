#include <Arduino.h>
#include <main.h>

// Define task handles
TaskHandle_t generate_handle;
TaskHandle_t serial_handle;
TaskHandle_t flash_handle;


// Initialize the two buffer arrays
CoordBuff coordBufs[LAYERS]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
BitLayerBuff bitBufs[LAYERS];    

void setup(){
  #if LOGGING 
    Serial.begin(9600);
  #endif
  // Initializes FreeRTOS
  #if LOGGING 
    Serial.println("Starting FreeRTOS scheduler");
  #endif
  // Initializes mutexes (buffer values automatically set to zero when declared as static variables)
  // for (int i = 0; i < LAYERS; i++) {
  //   coordBufs[i].mutex = xSemaphoreCreateMutex();
  //   bitBufs[i].mutex = xSemaphoreCreateMutex();
  // }
  // Create tasks
  // xTaskCreatePinnedToCore(
  //                   generate_task,     /* Task function. */
  //                   "generate",        /* name of task. */
  //                   STACK_SIZE,        /* Stack size of task */
  //                   NULL,              /* parameter of the task */
  //                   GENERATE_PRIORITY, /* priority of the task */
  //                   &generate_handle,  /* Task handle to keep track of created task */
  //                   0);                /* pin task to core 0 */     
  //  xTaskCreate(
  //                   serial_task,      /* Task function. */
  //                   "serial",         /* name of task. */
  //                   STACK_SIZE,       /* Stack size of task */
  //                   NULL,             /* parameter of the task */
  //                   SERIAL_PRIORITY,  /* priority of the task */
  //                   &serial_handle   /* Task handle to keep track of created task */
  //                   );               /* pin task to core 0 */                  
  // xTaskCreatePinnedToCore(
  //                   serial_task,      /* Task function. */
  //                   "serial",         /* name of task. */
  //                   STACK_SIZE,       /* Stack size of task */
  //                   NULL,             /* parameter of the task */
  //                   SERIAL_PRIORITY,  /* priority of the task */
  //                   &serial_handle,   /* Task handle to keep track of created task */
  //                   1);               /* pin task to core 0 */                  
  // xTaskCreatePinnedToCore(
  //                   flash_task,      /* Task function. */
  //                   "flash",         /* name of task. */
  //                   STACK_SIZE,      /* Stack size of task */
  //                   NULL,            /* parameter of the task */
  //                   FLASH_PRIORITY,  /* priority of the task */
  //                   &flash_handle,   /* Task handle to keep track of created task */
  //                   1);              /* pin task to core 1 */                  

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

  // char serialBuffer[91] = "173824481236384712536823764837285176143647326754831257248176832641285782473265814386427187";
  //inside while loop
  //ready to flag
 
  // point_t layer_points[LAYERS][MAX_POINTS_PER_LAYER];
  // int points_count[LAYERS];

  // uint8_t currLayer = 0;
  // bool readyToProcess = true;
  #if LABVIEW_EN
    char endMarker = '\n';
    int ndx = 0;
    readyToProcess = false;
  #endif
  while (1) {
    #if LOGGING
      Serial.println("Enters serial task");
    #endif
    #if SERIAL_TASK_EN
      if (readyToProcess) {
        // Process serial string using provided function from serial.c

        process_serial_string(serialBuffer, layer_points, points_count);
        //Serial.println((String)currLayer + (String)points_count[currLayer]);
        //peek then if statement to check if true/ if true skip
        if(xQueuePeek((xQueueHandle)coordBufs[currLayer].mutex,(void *)NULL,(portTickType)NULL) == pdTRUE){  
          
          // set MSB to 1
          // 2.1. Take the mutex (blocked if busy, change max delay to skip?)
          // xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY);

          // 2.2. Fill the coordinate buffers (Critical Section - try as short as possible)
          // for (int i = 0; i < points_count[currLayer]; i++) { //The number of points in the current layer is given here
          //   coordBufs[currLayer].points[i].x = layer_points[currLayer][i].x;
          //   /*This line assigns the x-coordinate value of the i-th point in the layer_points array for the current layer 
          //   to the x-coordinate value of the i-th point in the coordBufs array for the same layer*/
          //   coordBufs[currLayer].points[i].y = layer_points[currLayer][i].y;
          // }
          // coordBufs[currLayer].point_count = points_count[currLayer]; //store point_count locally
          /*assigns the total number of points in the current layer 
          to the point_count "field (the struct)" of the CoordBuff struct for the same layer.*/

          // 2.3. Return the mutex
          // xSemaphoreGive(coordBufs[currLayer].mutex);
        }
//shift 8 BIT LAYER BOOL RIGHT
        if (currLayer == LAYERS-1){
          currLayer = 0;
        } else{
          currLayer++;
        }
//SET UP AND OPERATOR BIT MASK TO DETERMINE IF ALL ARE PROCESSED
        //readyToProcess = ~(finish[1] & finish[2] & finish[3] ...);
      } 
      #if LABVIEW_EN
        else if (Serial.available()){ // non-blocking
          char ch = Serial.read();
          if (ch != endMarker) {
            serialBuffer[ndx] = ch;
            ndx++;
            if (ndx >= numChars) { // ring buffer loop-around mechanism
              ndx = 0;
            }
          } else {
            serialBuffer[ndx] = '\0'; // Null-terminate the string
            readyToProcess = true;
            ndx = 0;
          }
        }
      #endif
    #endif
    vTaskDelay(xDelay); // Delay 10 cycles
  }
}

// void generate_task(void *pvParameters) {
//   const TickType_t xDelay = pdMS_TO_TICKS(100);

//   uint8_t currLayer = 0;
//   bitset_t tempBitBuffer[ROWS]; 

//   while (1) {
//     #if LOGGING 
//       Serial.println("Enters generate task");
//     #endif
//     #if GENERATE_TASK_EN
//       // Take mutexes (blocked if any is unavailable)
//       xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // change max delay to skip?
//       // holding the mutex for coordBufs


//       // Convert coordinates to bitmaps (generate.c)

//       // Convert coordinates to bitmaps (generate.c)
//       fill_temp_buffer_with_coords(tempBitBuffer, &coordBufs[currLayer]);


//       // Return the mutex --> releasing mutex
//       xSemaphoreGive(coordBufs[currLayer].mutex);

//       // Take mutex for bitBufs
//       xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);

//       // Copy the data to bitBufs from tempBuffer
//       for (int i = 0; i < ROWS; i++) {
//         bitBufs[currLayer].buff[i] = tempBitBuffer[i];
//       }

//       // Return the mutex --> realsing mutex
//       xSemaphoreGive(bitBufs[currLayer].mutex);

//       if (currLayer >= LAYERS) {
//         currLayer = 0;
//       } else {
//         currLayer++;
//       }
//     #endif
//     vTaskDelay(xDelay); // Delay 100 milliseconds
//     // note that vTaskDelay is non-blocking (enable context switching), but the arduino delay() is blocking (i.e. the CPU will be stuck here)
//   }
// }


// void flash_task(void *pvParameters) {
//   // to run at a certain frequency
//   const TickType_t xFrequency = 10;
//   TickType_t xLastWakeTime;
//   BaseType_t xWasDelayed;
//   xLastWakeTime = xTaskGetTickCount();  // get initial time t0
//   uint8_t currLayer = 0;
//   while (1) {
//     #if LOGGING 
//       Serial.println("Enters flash task");
//     #endif
//     // Wait for the clock to tick
//     xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
//     #if FLASH_TASK_EN 
//       //void flash_task(void *pvParameters) 
//       xSemaphoreTake(bitBufs[currLayer].mutex, portMAX_DELAY);
//       layer_write(bitBufs[currLayer]);
//       xSemaphoreGive(bitBufs[currLayer].mutex);
//       //Tell 8 bit shift register to light new layer ********** //
//       digitalWrite(layer_clk,0);                                //
//       //check if flag is past 8 layers--> reset layer count     //
//       if(currLayer == 0) {                                     //
//         digitalWrite(Layer, 1);                                 //
//       } else {                                                  //
//         digitalWrite(Layer, 0);                                 //
//       }                                                         //
//       digitalWrite(layer_clk, 1);                               //
//       //********************************************************//
//       //update layer flag for upcoming layer
//       currLayer++;
//       //reset counter once it reaches final layer
//       if(currLayer == 8){
//         currLayer = 0;
//       }
//     #endif
//   }
// }