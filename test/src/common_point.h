#ifndef COMMON_POINT_H
#define COMMON_POINT_H
#include <Arduino.h>

#define ROW_SIZE 8
#define COL_SIZE 8
#define LAYER_SIZE 8
#define COORD_BUFF_SIZE 64 // The maximum number of points per layer is 64

/* DATA TYPE DEFINITIONS*/
typedef uint8_t bitset_t;

typedef struct {
    int x;
    int y;
} point_t;

typedef struct CoordBuff{
    point_t points[COORD_BUFF_SIZE]; //The CoordBuff struct contains an array of point_t
    uint8_t point_count; //to keep track of the number of points in the buffer, *we can remove*
    SemaphoreHandle_t mutex;
} CoordBuff;


typedef struct BitLayerBuff {               // bitBufs[0]: [bitrow1, bitrow2...], mutex
  uint8_t buff[ROW_SIZE];                 // bitBufs[1]: [bitrow1, bitrow2...], mutex
  SemaphoreHandle_t mutex;
} BitLayerBuff;


/* END DATA TYPE DEFINITIONS*/

#endif // COMMON_H