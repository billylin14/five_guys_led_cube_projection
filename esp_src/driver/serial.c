#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ROWS 8
#define COLS 8
#define LAYERS 8
#define MAX_POINTS_PER_LAYER 64 //The maximum number of points per layer is 64

//layer_pointsis a struct of x,y with 8 by 64 array.

typedef struct {
    int x;
    int y;
} point_t;

int main() {
    const char *serial_string = "173824481236384712536823764837285176143647326754831257248176832641285782473265814386427187483732512847238567124873245782481263746831587274832653785781236481523785468123751834";
    point_t layer_points[LAYERS][MAX_POINTS_PER_LAYER];
    int points_count[LAYERS];

    process_serial_string(serial_string, layer_points, points_count);

    for (int z = 0; z < LAYERS; z++) {
        printf("Layer %d:\n", z);
        for (int i = 0; i < points_count[z]; i++) {
            printf("(%d, %d)\n", layer_points[z][i].x, layer_points[z][i].y);
        }
    }

    return 0;
}

void process_serial_string(const char *serial_string, point_t layer_points[LAYERS][MAX_POINTS_PER_LAYER], int points_count[LAYERS]) {
    size_t len = strlen(serial_string);

    for (int i = 0; i < LAYERS; i++) {
        points_count[i] = 0; //initialized to all zeros at the beginning
    } //

    for (size_t i = 0; i + 2 < len; i += 3) { //i in order of xyz* 
        int x = serial_string[i] - '0';
        int y = serial_string[i + 1] - '0';
        int z = serial_string[i + 2] - '0'; //The variable z in the code represents the current layer



        if (x >= 0 && x < ROWS && y >= 0 && y < COLS && z >= 0 && z < LAYERS) {
            layer_points[z][points_count[z]].x = x; //The .x at the end means assigning the value of x to the x field of that point_t struct
            layer_points[z][points_count[z]].y = y; //z itself can represent number of x,y pairs
            points_count[z]++; //points_count[z] refers to the count of points added to that specific layer so far
            //whenever a new point is added to a layer, points_count[z] is incremented by one
            // count is updated each time a new point is added to the layer z
        }
    }
}