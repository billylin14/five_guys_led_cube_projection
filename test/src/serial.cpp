#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "serial.h"



//layer_pointsis a struct of x,y with 8 by 64 array.

void process_serial_string(char *serial_string, point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE], int points_count[LAYER_SIZE]) {
    size_t len = strlen(serial_string);

    for (int i = 0; i < LAYER_SIZE; i++) {
        points_count[i] = 0; //initialized to all zeros at the beginning
    } 

    for (size_t i = 0; i < len-1; i += 3) { //i in order of xyz* 
        int x = serial_string[i] - '1';
        int y = serial_string[i + 1] - '1';
        int z = serial_string[i + 2] - '1'; //The variable z in the code represents the current layer

        if (x >= 0 && x < ROW_SIZE && y >= 0 && y < COL_SIZE && z >= 0 && z < LAYER_SIZE) {
            layer_points[z][points_count[z]].x = x; //The .x at the end means assigning the value of x to the x field of that point_t struct
            layer_points[z][points_count[z]].y = y; //z itself can represent number of x,y pairs
            if (points_count[z] == COORD_BUFF_SIZE) {
                Serial.println("OOPS");
            } else {
                points_count[z]++; 
            }
            // points_count[z]++; //points_count[z] refers to the count of points added to that specific layer so far
            //whenever a new point is added to a layer, points_count[z] is incremented by one
            // count is updated each time a new point is added to the layer z
        }
    }
}