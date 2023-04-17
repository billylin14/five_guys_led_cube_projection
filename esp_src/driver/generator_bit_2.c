//Arthor: Junwon Hwang & Michael Wang
//If any code changes, please contact --both-- of us. 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define ROWS 8
#define COLS 8
#define BUFFER_SIZE ROWS * COLS

// Define a bitset type using an array of 8 or 64-bit integers
typedef uint8_t bitset_t; //defines bitset_t to be equivalent to uint8_t or uint64_t


// pointer *bitset requires to have a data type in front of it
// sets a particular bit in a bitset_t variable

// Function to set the bit at the given position in the bitset
void set_bit(bitset_t *bitset, int pos) {
    *bitset |= (1ull << pos);
}

/*
The function sets the bit (1) at position pos by performing a bitwise OR operation between 
the current value of *bitset and a mask created by shifting an unsigned long long (ull) value with a 1 
in the position of the bit to be set (1ull << pos)
This sets the bit to 1 without affecting the values of other bits in the bitset
*/

//*bitset |= (1ull << pos) sets the pos-th bit of *bitset to 1 while leaving all other bits unchanged

// Function to clear the bit at the given position in the bitset
void clear_bit(bitset_t *bitset, int pos) {
    *bitset &= ~(1ull << pos);
}


// Function to check if the bit at the given position in the bitset is set
bool is_bit_set(bitset_t *bitset, int pos) {
    return (*bitset & (1ull << pos)) != 0;

    //If 1 is assigned at the position, it sends true, meaning the bit is already taken, and it send entire line of bits
}

int main() {
    srand(time(NULL));

    bitset_t buffer[ROWS];
    for (int i = 0; i < ROWS; i++) {
        buffer[i] = 0;
    }

    int z = 0;
    int prev_z = -1;

    // Read from the sorted xyz file
    FILE *file;
    file = fopen("coordinates_update.xyz", "r");

    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }

    while (!feof(file)) {
        // Read the x, y, and z values from the file
        int x, y, current_z;
        fscanf(file, "%d %d %d", &x, &y, &current_z);

        // Check if the z value has changed
        if (current_z != prev_z) {
            // Reset the buffer
            for (int i = 0; i < ROWS; i++) {
                buffer[i] = 0;
            }
            z = current_z;

            //memset(ptr, size, value)**

            printf("Next Layer! \n");
        }

        // Set the bit in the buffer corresponding to the x and y coordinates
        set_bit(&buffer[x], y);

        // Print the buffer
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                printf("%d ", is_bit_set(&buffer[i], j));
            }
            printf("\n");
        }
        printf("\n");

        // Delay for 0.5 seconds
        usleep(500000);


        prev_z = current_z;
    }

    fclose(file);

    return 0;


}