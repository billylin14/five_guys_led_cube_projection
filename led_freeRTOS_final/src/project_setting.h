#ifndef PROJECT_SETTING_H
#define PROJECT_SETTING_H

/* SERIAL CONFIG */
#define BAUD_RATE 115200

/* freeRTOS CONFIG */
#define STACK_SIZE 10000
#define SERIAL_PRIORITY 2
#define GENERATE_PRIORITY 1
#define FLASH_PRIORITY 1

/* IF GUARDS ON PROCESSING */
#define LABVIEW 1
#define LOGGING 0//(1&&!LABVIEW)
#define SERIAL_TASK_EN 1
#define GENERATE_TASK_EN 1
#define FLASH_TASK_EN 1

/* CUBE SIZE CONFIG */
#define ROW_SIZE 8
#define COL_SIZE 8
#define LAYER_SIZE 8
#define COORD_BUFF_SIZE ROW_SIZE*COL_SIZE // The maximum number of points per layer is row * col
#define NUM_CHARS 512

#endif