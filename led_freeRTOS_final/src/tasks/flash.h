#ifndef DATA_TYPES
#define DATA_TYPES
#include "../data.h"
#endif

#ifndef FLASH_H
#define FLASH_H

#define GetBit(var, bit) ((var & (1 << bit)) != 0) //Get bit in byte function

extern TaskHandle_t flash_handle;

void flash_task(void *pvParameters);
void layer_write(BitLayerBuff *bitBufs);

#endif