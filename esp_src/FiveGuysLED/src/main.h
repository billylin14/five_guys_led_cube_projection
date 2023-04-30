#ifndef MAIN_H // this is the header guard
#define MAIN_H

#include <stdint.h>

/* task function includes */
#include "common_point.h"
#include "serial.h"
#include "generate.h"
#include "driver.h"

// debug enabler (to enable/disable before compiling)
#define LOGGING 1
#define SERIAL_TASK_EN 0
#define GENERATE_TASK_EN 0
#define FLASH_TASK_EN 0
#define LABVIEW_EN 0

// the higher the number the higher the priority it is
#define GENERATE_PRIORITY 2
#define SERIAL_PRIORITY 1
#define FLASH_PRIORITY 2

/* freeRTOS configs */
#define STACK_SIZE 2048



/*
extern TaskHandle_t generate_handle;
extern TaskHandle_t serial_handle;
extern TaskHandle_t flash_handle;
*/


/* ARRAY INITIALIZATION (notrecommended)*/ 
//CoordBuff coordBufs[LAYERS]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
//BitLayerBuff bitBufs[LAYERS];     
/* END ARRAY INITIALIZATION*/



// extern: variable is defined in another translation unit
// static: variable is only visible within the translation unit where it is defined --> conflict

// functions
void generate_task(void *pvParameters);
void serial_task(void *pvParameters);
void flash_task(void *pvParameters);

#endif