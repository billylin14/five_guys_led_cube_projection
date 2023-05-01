#ifndef DATA_TYPES
#define DATA_TYPES
#include "../data.h"
#endif

#ifndef GENERATE_H 
#define GENERATE_H

//void set_bit(bitset_t *bitset, int pos);
#define SetBitOn(var, bit) (var |= (1 << bit))
#define GetBit(var, bit) ((var & (1 << bit)) != 0) //Get bit in byte function

extern TaskHandle_t generate_handle;

void clear_bit(bitset_t *bitset, int pos);
bool is_bit_set(bitset_t *bitset, int pos);
void fill_temp_buffer_with_coords(bitset_t tempBuffer[ROW_SIZE], CoordBuff *coordBuff);
void generate_task(void *pvParameters);

#endif // GENERATE_H

