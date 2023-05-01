#include <Arduino.h>
#include "serial.h"

#include "../project_setting.h"
#include "serial.h"
#include "data.h"

TaskHandle_t serial_handle;

void serial_task(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(100);  // 100 clock cycle (placeholder)
    uint8_t currLayer = 0;
    bool readyToProcess = false;

    // local temporary buffers
    char serialBuffer[NUM_CHARS+1];
    point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE];
    int points_count[LAYER_SIZE];

    #if LABVIEW
    unsigned int ndx = 0;
    char endMarker = '\n';
    #else
    const char *numString = "173824481236384712536823764837285176";
    strcpy(serialBuffer, numString);  // Copy the numString into serialBuffer
    #endif

    while (1) {
        #if LOGGING
        Serial.println("Enters serial task");
        #endif
        #if SERIAL_TASK_EN
        // read from serial buffer, convert one coord string to int x, y, z at a time
        // store the int x, y, z in a temperary buffer
        // move this x,y,z into the coordBufs
        #if !LABVIEW
        if (!readyToProcess) {
            process_serial_string(serialBuffer, layer_points, points_count);
            readyToProcess = true;
        }
        #endif
        if (readyToProcess) {
            #if LOGGING
            Serial.printf("Current Layer: %d\n", currLayer);  
            Serial.printf("Enters serial task, point count for current layer: %d\n", points_count[currLayer]);
            #endif
            xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); // Take mutex of the currLayer (**Critical Section**)
            // Fill the coordinate buffers 
            /*This line assigns the x-coordinate value of the i-th point in the layer_points array for the current layer 
            to the x-coordinate value of the i-th point in the coordBufs array for the same layer*/
            for (int i = 0; i < points_count[currLayer]; i++) { // loop through the number of points in the current layer
                coordBufs[currLayer].points[i].x = layer_points[currLayer][i].x;
                coordBufs[currLayer].points[i].y = layer_points[currLayer][i].y;
            }
            coordBufs[currLayer].point_count = points_count[currLayer];
            xSemaphoreGive(coordBufs[currLayer].mutex);

            #if LOGGING
            // Print layer points and coordbuffs
            for (int i = 0; i < points_count[currLayer]; i++) {
                Serial.printf("Current Layer_Points: (%d, %d)\n", layer_points[currLayer][i].x, layer_points[currLayer][i].y);
            }
            Serial.printf("\n");
            for (int i = 0; i < points_count[currLayer]; i++) {
                Serial.printf("From CoordBuffs(%d, %d, %d)\n", coordBufs[currLayer].points[i].x, coordBufs[currLayer].points[i].y, currLayer);
            }
            Serial.printf("\n");
            Serial.printf("\n");
            #endif
            
            if (currLayer >= LAYER_SIZE - 1) {
                currLayer = 0;
                #if LOGGING
                Serial.printf("Current Layer: %d\n", currLayer);  
                Serial.printf("\n");
                #endif
                readyToProcess = false;
            } else {
            currLayer++;
            #if LOGGING
            Serial.printf("Current Layer: %d\n", currLayer); 
            Serial.printf("\n");
            #endif
            }
            
        }
        #if LABVIEW
        else {
            while (Serial.available() > 0) { // non-blocking
                char ch = Serial.read();
                #if LABVIEW
                Serial.print(ch);
                #endif
                if (ch != endMarker) {
                    serialBuffer[ndx] = ch;
                    ndx++;
                    if (ndx >= NUM_CHARS) { // ring buffer loop-around mechanism
                        ndx = 0;
                    }
                } else {
                    serialBuffer[ndx] = '\0';
                    process_serial_string(serialBuffer, layer_points, points_count);
                    readyToProcess = true;
                    ndx = 0;

                }
            }
        }
        #endif
        #endif
        vTaskDelay(xDelay); // Delay 100 cycle
    }
}

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
            if (points_count[z] != COORD_BUFF_SIZE) {
                points_count[z]++; 
            } 
            #if LOGGING
            else {
                Serial.println("OOPS");
            }
            #endif
            /* points_count[z] refers to the count of points added to that specific layer so far
            //whenever a new point is added to a layer, points_count[z] is incremented by one
            // count is updated each time a new point is added to the layer z */
        }
    }
}