#include <Arduino.h>

#include "../project_setting.h"
#include "generate.h"

TaskHandle_t generate_handle;

void generate_task(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(50); //1/2 the delay time of serial
  uint8_t currLayer = 0;
  bitset_t tempBuffer[ROW_SIZE]; // Temporary storage for buffer

  while (1) {
    #if LOGGING 
    Serial.println("Enters generate task");
    #endif
    #if GENERATE_TASK_EN
    xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // take the mutex of a layer (*critical section*)
    fill_temp_buffer_with_coords(tempBuffer, &coordBufs[currLayer]); // Convert coordinates to bitmaps
    xSemaphoreGive(coordBufs[currLayer].mutex);
    
    #if LOGGING
    Serial.printf("Generate function ran %d layer \n", currLayer); // print current buffer layer
    #endif

    // moves temp buffer to bitBufs for flashing data
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

void clear_bit(bitset_t *bitset, int pos) {
    *bitset &= ~(1ull << pos);
}

bool is_bit_set(bitset_t *bitset, int pos) {
    return (*bitset & (1ull << pos)) != 0;
}
//layer_points is a struct of x,y with 8 by 64 array.

void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROW_SIZE], CoordBuff *coordBuff) {
    for (int i = 0; i < ROW_SIZE; i++) {
        tempBuffer[i] = 0;
    }

    for (int i = 0; i < coordBuff->point_count; i++) {
        int x = coordBuff->points[i].x;
        int y = coordBuff->points[i].y;
        SetBitOn(tempBuffer[x], y);
    }
}
