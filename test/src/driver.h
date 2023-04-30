//main header guard
#ifndef DRIVER_H
#define DRIVER_H

#include "common_point.h"

//define pin outs
#define Layer 4
#define Row1 13
#define Row2 12
#define Row3 27
#define Row4 33
#define Row5 15
#define Row6 32
#define Row7 14
#define Row8 22
#define row_clk 21
#define layer_clk 25

//define macros for setting and getting bit
#define GetBit(var, bit) ((var & (1 << bit)) != 0) //Get bit in byte function


#define TEST 0

//functions
void readlight_layer(void *pvParameters);
void layer_write(BitLayerBuff bitBufs);

#endif