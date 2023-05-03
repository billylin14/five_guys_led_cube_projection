#include <Arduino.h>

#include "../project_setting.h"
#include "../pin_def.h"
#include "flash.h"

// Define task handles
TaskHandle_t flash_handle;
const int delay_us = 0;

void flash_task(void *pvParameters) {
    // to run at a certain frequency
    const TickType_t xFrequency = 1;
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

        digitalWrite(layer_clk,0);
        delayMicroseconds(delay_us);                                

        if(currLayer == 0) {                                     
        digitalWrite(Layer, 1); 
        delayMicroseconds(delay_us);                                    
        } else {                                                 
        digitalWrite(Layer, 0);
        delayMicroseconds(delay_us);                                          
        }                                                         
        digitalWrite(layer_clk, 1);  
        delayMicroseconds(delay_us);                              

        currLayer++;

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
        digitalWrite(Row1, GetBit(bitBufs->buff[row_flag],0)); //flash [row_flag][1st column] bit each digital wrte is 3.4E-03
        delayMicroseconds(delay_us); 
        digitalWrite(Row2, GetBit(bitBufs->buff[row_flag],1)); //flash [row_flag][2st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row3, GetBit(bitBufs->buff[row_flag],2)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row4, GetBit(bitBufs->buff[row_flag],3)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row5, GetBit(bitBufs->buff[row_flag],4)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row6, GetBit(bitBufs->buff[row_flag],5)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row7, GetBit(bitBufs->buff[row_flag],6)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
        digitalWrite(Row8, GetBit(bitBufs->buff[row_flag],7)); //flash [row_flag][3st column] bit
        delayMicroseconds(delay_us); 
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
        //delayMicroseconds(delay_us); 
    }
}