#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "generate.h"
#include "main.h"

#define ROWS 8
#define COLS 8
#define LAYERS 8
#define MAX_POINTS 64 //The maximum number of points per layer is 64

//layer_pointsis a struct of x,y with 8 by 64 array.


typedef struct {
    int x;
    int y;
} Point;

typedef uint8_t bitset_t;

void set_bit(bitset_t *bitset, int pos) {
    *bitset |= (1ull << pos);
}

void clear_bit(bitset_t *bitset, int pos) {
    *bitset &= ~(1ull << pos);
}

bool is_bit_set(bitset_t *bitset, int pos) {
    return (*bitset & (1ull << pos)) != 0;
}

void process_serial_string(const char *str, Point layer_points[LAYERS][MAX_POINTS], int points_count[LAYERS]) {
    int x, y, z;
    for (size_t i = 0; i < strlen(str); i += 3) { //i in order of xyz* 
        x = str[i] - '0';
        y = str[i + 1] - '0';
        z = str[i + 2] - '0'; //The variable z in the code represents the current layer

        if (x >= 0 && x < ROWS && y >= 0 && y < COLS && z >= 0 && z < LAYERS) {
            layer_points[z][points_count[z]].x = x; //The .x at the end means assigning the value of x to the x field of that point_t struct
            layer_points[z][points_count[z]].y = y;
            points_count[z]++; //points_count[z] refers to the count of points added to that specific layer so far
            //whenever a new point is added to a layer, points_count[z] is incremented by one
            // count is updated each time a new point is added to the layer 
        }
    }
}

int main() {
    const char *serial_string = "5242066522345463764240205344202236472306256645061352731115064700063436276037035372664523123176241317156103454616172056352372246364666166370726405462467527326272374555430647434745434264240456325626715036255..."; // Your serial string

    Point layer_points[LAYERS][MAX_POINTS];
    int points_count[LAYERS] = {0}; //initialized to all zeros at the beginning

    process_serial_string(serial_string, layer_points, points_count);

    // Print layer_points arrays
    printf("Layer Points Arrays:\n");
    for (int z = 0; z < LAYERS; z++) {
        printf("Layer %d:\n", z);
        for (int i = 0; i < points_count[z]; i++) {
            printf("(%d, %d)\n", layer_points[z][i].x, layer_points[z][i].y);
        }
    }

    // Convert layer_points arrays to buffer arrays
    bitset_t buffer[LAYERS][ROWS];
    for (int z = 0; z < LAYERS; z++) {
        for (int i = 0; i < ROWS; i++) {
            buffer[z][i] = 0;
        }

        for (int i = 0; i < points_count[z]; i++) {
            int x = layer_points[z][i].x;
            int y = layer_points[z][i].y;
            set_bit(&buffer[z][x], y);
        }
    }

    // Print buffer arrays
    printf("Buffer Arrays:\n");
    for (int z = 0; z < LAYERS; z++) {
        printf("Layer %d:\n", z);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                printf("%d ", is_bit_set(&buffer[z][i], j));
            }
            printf("\n");
        }
    }

    return 0;
}


void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROWS], CoordBuff *coordBuff) {
    for (int i = 0; i < ROWS; i++) {
        tempBuffer[i] = 0;
    }

    for (int i = 0; i < coordBuff->point_count; i++) {
        int x = coordBuff->points[i].x;
        int y = coordBuff->points[i].y;
        set_bit(&tempBuffer[x], y);
    }
}
