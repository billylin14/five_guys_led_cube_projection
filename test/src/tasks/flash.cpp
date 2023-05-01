#include <Arduino.h>

#include "../project_setting.h"
#include "../pin_def.h"
#include "flash.h"

// Define task handles
TaskHandle_t flash_handle;

void flash_task(void *pvParameters) {
    // to run at a certain frequency
    const TickType_t xFrequency = 30 / portTICK_PERIOD_MS;
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();  // get initial time t0

    uint8_t currLayer = 0;

    while (1) {
        #if LOGGING 
        Serial.println("Enters flash task");
        #endif

        #if FLASH_TASK_EN 
        #if LOGGING
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
        digitalWrite(layer, 1);                                 //
        } else {                                                  //
        digitalWrite(layer, 0);                                 //
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
void layer_write(BitLayerBuff *bitBufs) {
    for (int row_flag=0; row_flag<ROW_SIZE; row_flag++){
        // data is shifted on rising edge
        digitalWrite(row_clk, 0);
        //simplify by using for loop
        digitalWrite(row1, GetBit(bitBufs->buff[row_flag],0)); //flash [row_flag][1st column] bit each digital wrte is 3.4E-03
        digitalWrite(row2, GetBit(bitBufs->buff[row_flag],1)); //flash [row_flag][2st column] bit
        digitalWrite(row3, GetBit(bitBufs->buff[row_flag],2)); //flash [row_flag][3st column] bit
        digitalWrite(row4, GetBit(bitBufs->buff[row_flag],3)); //flash [row_flag][3st column] bit
        digitalWrite(row5, GetBit(bitBufs->buff[row_flag],4)); //flash [row_flag][3st column] bit
        digitalWrite(row6, GetBit(bitBufs->buff[row_flag],5)); //flash [row_flag][3st column] bit
        digitalWrite(row7, GetBit(bitBufs->buff[row_flag],6)); //flash [row_flag][3st column] bit
        digitalWrite(row8, GetBit(bitBufs->buff[row_flag],7)); //flash [row_flag][3st column] bit
        #if LOGGING
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],0));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],1));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],2));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],3));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],4));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],5));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],6));
        Serial.printf("%d", GetBit(bitBufs->buff[row_flag],7));
        Serial.printf("\n");
        #endif
        digitalWrite(row_clk, 1);
    }
}