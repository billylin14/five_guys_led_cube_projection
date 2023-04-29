// this is the header guard
#ifndef GENERATE_H
#define GENERATE_H

// contains includes of the serial reading
// contains the buffers of the serial buffer
// contains the function definition of the serial read function (should be called in the freeRTOS)
#ifndef GENERATE_H
#define GENERATE_H

#include <stdint.h>
#include "main.h"

void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROWS], CoordBuff *coordBuff);

#endif // GENERATE_H
