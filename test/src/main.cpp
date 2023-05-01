#include <Arduino.h>
#include <esp_task_wdt.h>

/* project configs and data declarations*/
#include "project_setting.h"
#include "data.h"
#include "pin_def.h"

/* task includes */
#include "./tasks/serial.h"
#include "./tasks/generate.h"
#include "./tasks/flash.h"   

CoordBuff coordBufs[LAYER_SIZE]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
BitLayerBuff bitBufs[LAYER_SIZE]; 

void pin_init() {
  pinMode(layer, OUTPUT);
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(row5, OUTPUT);
  pinMode(row6, OUTPUT);
  pinMode(row7, OUTPUT);
  pinMode(row8, OUTPUT);
  pinMode(row_clk, OUTPUT);
  pinMode(layer_clk, OUTPUT); 
}

void setup() {
  Serial.begin(BAUD_RATE);
  esp_task_wdt_init(10, false); // Increase the timeout to 10 seconds
  pin_init();
  // Initializes mutexes (buffer values automatically set to zero when declared as static variables)
  for (int i = 0; i < LAYER_SIZE; i++) {
    coordBufs[i].mutex = xSemaphoreCreateMutex();
    bitBufs[i].mutex = xSemaphoreCreateMutex();
  }
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    generate_task,     /* Task function. */
                    "generate",        /* name of task. */
                    10000,             /* Stack size of task */
                    NULL,              /* parameter of the task */
                    GENERATE_PRIORITY, /* priority of the task */
                    &generate_handle,  /* Task handle to keep track of created task */
                    1);                /* pin task to core # */                  
  xTaskCreatePinnedToCore(
                    serial_task,      /* Task function. */
                    "serial",         /* name of task. */
                    STACK_SIZE,       /* Stack size of task */
                    NULL,             /* parameter of the task */
                    SERIAL_PRIORITY,  /* priority of the task */
                    &serial_handle,   /* Task handle to keep track of created task */
                    1);               /* pin task to core # */                  
  xTaskCreatePinnedToCore(
                    flash_task,      /* Task function. */
                    "flash",         /* name of task. */
                    STACK_SIZE,      /* Stack size of task */
                    NULL,            /* parameter of the task */
                    FLASH_PRIORITY,  /* priority of the task */
                    &flash_handle,   /* Task handle to keep track of created task */
                    0);              /* pin task to core # */ 
}

void loop() {
  
}



