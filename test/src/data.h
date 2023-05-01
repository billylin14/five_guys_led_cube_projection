#ifndef PROJECT_SETTING
#define PROJECT_SETTING
#include "project_setting.h"
#endif

#ifndef DATA_H
#define DATA_H

/* DATA TYPE DEFINITIONS*/
typedef uint8_t bitset_t;

typedef struct {
    int x;
    int y;
} point_t;

typedef struct CoordBuff{
    point_t points[COORD_BUFF_SIZE]; //The CoordBuff struct contains an array of point_t
    uint8_t point_count;
    SemaphoreHandle_t mutex;
} CoordBuff;


typedef struct BitLayerBuff {               // bitBufs[0]: [bitrow1, bitrow2...], mutex
  uint8_t buff[ROW_SIZE];                 // bitBufs[1]: [bitrow1, bitrow2...], mutex
  SemaphoreHandle_t mutex;
} BitLayerBuff;


/* VARIABLE DECLARATION */
extern CoordBuff coordBufs[LAYER_SIZE]; // coordBufs = [{[[x1,y1], [x2,y2], ...], mutex}, {[[x1,y1], [x2,y2], ...], mutex}, ...]
extern BitLayerBuff bitBufs[LAYER_SIZE];    

#endif