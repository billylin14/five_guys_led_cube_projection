// this is the header guard
#ifndef MAIN_H
#define MAIN_H

// debug enabler (to enable/disable before compiling)
#define LOGGING 1
#define SERIAL_TASK_EN 0
#define GENERATE_TASK_EN 0
#define FLASH_TASK_EN 0

// the higher the number the higher the priority it is
#define tskGENERATE_PRIORITY 2
#define tskSERIAL_PRIORITY 1
#define tskFLASH_PRIORITY 1

/* freeRTOS configs */
#define STACK_SIZE 1000

/* task function includes */
#include "./driver/generate.h"
#include "./driver/serial.h"
#include "./driver/flash.h"

/* MACROS */
#define LAYER_SIZE 8
#define ROW_SIZE 8
#define COORD_BUF_SIZE ROW_SIZE*ROW_SIZE

/* DECLARATIONS */

// variables
extern TaskHandle_t generate_handle;
extern TaskHandle_t serial_handle;
extern TaskHandle_t flash_handle;

typedef struct CoordLayerBuff {             // coordBufs[0]: {[[x1,y1], [x2,y2], ...], mutex}
  uint8_t buff[COORD_BUF_SIZE][2];          // coordBufs[1]: {[[x1,y1], [x2,y2], ...], mutex}
  SemaphoreHandle_t mutex;
} CoordLayerBuff;

typedef struct BitLayerBuff {               // bitBufs[0]: [bitrow1, bitrow2...], mutex
  uint8_t buff[ROW_SIZE];                 // bitBufs[1]: [bitrow1, bitrow2...], mutex
  SemaphoreHandle_t mutex;
} BitLayerBuff;

extern static CoordLayerBuff coordBufs[LAYER_SIZE]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
extern static BitLayerBuff bitBufs[LAYER_SIZE];            

// functions
void generate_task(void *pvParameters);
void serial_task(void *pvParameters);
void flash_task(void *pvParameters);

#endif