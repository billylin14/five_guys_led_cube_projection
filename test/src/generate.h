#ifndef GENERATE_H // this is the header guard
#define GENERATE_H

#include <stdint.h>
#include "common_point.h"

void set_bit(bitset_t *bitset, int pos);

void clear_bit(bitset_t *bitset, int pos);

bool is_bit_set(bitset_t *bitset, int pos);

void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROW_SIZE], CoordBuff *coordBuff);

#endif // GENERATE_H

