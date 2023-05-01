#ifndef DATA_TYPES
#define DATA_TYPES
#include "../data.h"
#endif

#ifndef SERIAL_H
#define SERIAL_H

#define NUM_CHARS 512

extern TaskHandle_t serial_handle;

void serial_task(void *pvParameters);
void process_serial_string(char *serial_string, point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE], int points_count[LAYER_SIZE]);
#endif