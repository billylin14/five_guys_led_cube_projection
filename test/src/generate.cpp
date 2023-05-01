#include <stdbool.h>
#include <string.h>
#include "generate.h"


// void set_bit(bitset_t *bitset, int pos) {
//     *bitset |= (1ull << pos);
// }

void clear_bit(bitset_t *bitset, int pos) {
    *bitset &= ~(1ull << pos);
}

bool is_bit_set(bitset_t *bitset, int pos) {
    return (*bitset & (1ull << pos)) != 0;
}
//layer_pointsis a struct of x,y with 8 by 64 array.

void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROW_SIZE], CoordBuff *coordBuff) {
    for (int i = 0; i < ROW_SIZE; i++) {
        tempBuffer[i] = 0;
    }

    for (int i = 0; i < coordBuff->point_count; i++) {
        int x = coordBuff->points[i].x;
        int y = coordBuff->points[i].y;
        SetBitOn(tempBuffer[x], y);
    }
}
