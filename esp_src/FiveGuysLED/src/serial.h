#ifndef SERIAL_H //serial guard
#define SERIAL_H

#include "common_point.h"

//semphr.h header file includes the definitions and functions for semaphores 
//and other synchronization primitives provided by FreeRTOs


//extern SoftwareSerial port;
//extern const uint8_t numChars;
//extern char receivedChars[];   // an array to store the received data

// contains includes of the serial reading
// contains the buffers of the serial buffer
// contains the function definition of the serial read function (should be called in the freeRTOS)


// The function declaration for process_serial_string
void process_serial_string(char *serial_string, point_t layer_points[LAYERS][MAX_POINTS_PER_LAYER], int points_count[LAYERS]);

#endif