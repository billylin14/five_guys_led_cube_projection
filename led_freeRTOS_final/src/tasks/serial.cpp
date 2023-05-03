#include <Arduino.h>
#include "serial.h"

#include "../project_setting.h"
#include "serial.h"
#include "../data.h"

TaskHandle_t serial_handle;

void serial_task(void *pvParameters) {
    const TickType_t xDelay = 2;
    uint8_t currLayer = 0;
    bool readyToProcess = false;

    // local temporary buffers
    char serialBuffer[NUM_CHARS+1];
    point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE];
    int points_count[LAYER_SIZE];

    #if LABVIEW
    unsigned int ndx = 0;
    char endMarker = '>';
    #else
    const char *numString = "121131141151161171181191112122132142152162172182192";
    strcpy(serialBuffer, numString); 
    #endif

    while (1) {
        #if LOGGING
        Serial.println("Enters serial task");
        #endif
        #if SERIAL_TASK_EN

        #if !LABVIEW
        if (!readyToProcess) {
            process_serial_string(serialBuffer, layer_points, points_count);
            readyToProcess = true;
        }
        #endif
        if (readyToProcess) {

            xSemaphoreTake(coordBufs[currLayer].mutex, portMAX_DELAY); 
            for (int i = 0; i < points_count[currLayer]; i++) { 
                coordBufs[currLayer].points[i].x = layer_points[currLayer][i].x;
                coordBufs[currLayer].points[i].y = layer_points[currLayer][i].y;
            }
            coordBufs[currLayer].point_count = points_count[currLayer];
            xSemaphoreGive(coordBufs[currLayer].mutex);
            
            if (currLayer >= LAYER_SIZE - 1) {
                currLayer = 0;
                readyToProcess = false;

            } else {
            currLayer++;
            }
            
        }
        #if LABVIEW
        else {
            while (Serial.available() > 0) { 
                char ch = Serial.read();
                #if LABVIEW
                Serial.print(ch);
                #endif
                if (ch != endMarker) {
                    serialBuffer[ndx] = ch;
                    ndx++;
                    if (ndx >= NUM_CHARS-1) { 
                        ndx = 0;
                    }
                } else {
                    // serialBuffer[ndx] = '\0';
                    process_serial_string(serialBuffer, ndx, layer_points, points_count);
                    readyToProcess = true;
                    ndx = 0;

                }
            }
        }
        #endif
        #endif

        vTaskDelay(xDelay); 
    }
}

void process_serial_string(char *serial_string, unsigned int len, point_t layer_points[LAYER_SIZE][COORD_BUFF_SIZE], int points_count[LAYER_SIZE]) {
    // size_t len = strlen(serial_string);

    for (int i = 0; i < LAYER_SIZE; i++) {
        points_count[i] = 0; 
    } 

    for (size_t i = 0; i < len-1; i += 3) { 
        int x = serial_string[i] - '1';
        int y = serial_string[i + 1] - '1';
        int z = serial_string[i + 2] - '1'; 

        if (x >= 0 && x < ROW_SIZE && y >= 0 && y < COL_SIZE && z >= 0 && z < LAYER_SIZE) {
            layer_points[z][points_count[z]].x = x; 
            layer_points[z][points_count[z]].y = y; 
            if (points_count[z] != COORD_BUFF_SIZE) {
                points_count[z]++; 
            } 
            #if LOGGING
            else {
                Serial.println("OOPS");
            }
            #endif

        }
    }
}