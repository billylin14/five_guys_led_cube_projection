// this is the header guard
#ifndef SERIAL_H
#define SERIAL_H

#include "FreeRTOS.h"
#include "semphr.h"

//semphr.h header file includes the definitions and functions for semaphores 
//and other synchronization primitives provided by FreeRTOs

#include "stdint.h"

extern SoftwareSerial port;
extern const uint8_t numChars;
extern char receivedChars[];   // an array to store the received data

// contains includes of the serial reading
// contains the buffers of the serial buffer
// contains the function definition of the serial read function (should be called in the freeRTOS)
#define ROWS 8
#define COLS 8
#define LAYERS 8
#define MAX_POINTS_PER_LAYER 64 // The maximum number of points per layer is 64

typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    point_t points[MAX_POINTS_PER_LAYER];
    uint8_t point_count; //to keep track of the number of points in the buffer,
    SemaphoreHandle_t mutex;
} CoordBuff;

// The function declaration for process_serial_string
void process_serial_string(const char *serial_string, point_t layer_points[LAYERS][MAX_POINTS_PER_LAYER], int points_count[LAYERS]);

#endif