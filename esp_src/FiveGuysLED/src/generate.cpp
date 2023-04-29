#include <stdbool.h>
#include <string.h>
#include "generate.h"



//layer_pointsis a struct of x,y with 8 by 64 array.
void set_bit(bitset_t *bitset, int pos) {
    *bitset |= (1ull << pos);
}

void clear_bit(bitset_t *bitset, int pos) {
    *bitset &= ~(1ull << pos);
}

bool is_bit_set(bitset_t *bitset, int pos) {
    return (*bitset & (1ull << pos)) != 0;
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