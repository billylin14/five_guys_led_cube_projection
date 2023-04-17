//Arthor: Junwon Hwang & Michael Wang
//If any code changes, please contact --both-- of us. 

#include <stdio.h>
#include <stdlib.h>

#define NUM_SETS 8 // Define the number of sets as a constant
// required to sort the coordinates into 8 different sets based on the increasing order of z from 0 to 7

int sizes[NUM_SETS] = {0}; // Initialize the sizes array to all 0s

typedef struct {
    int x;
    int y;
    int z;
} Coordinate;

int compare_coordinates(const void *a, const void *b) {
    Coordinate *c1 = (Coordinate*)a;
    Coordinate *c2 = (Coordinate*)b;
    return c1->z - c2->z;
}

int main() {
    FILE *fp = fopen("coordinates.xyz", "r"); // open the file for reading
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    Coordinate coordinates[1000];

    // This line declares an array called coordinates that can hold 100 elements of the Coordinate struct type

    int i = 0;
    while (fscanf(fp, "%d %d %d", &coordinates[i].x, &coordinates[i].y, &coordinates[i].z) == 3) {
        i++;

        // Above condition that checks if fscanf() successfully read in 3 integer values from the input file fp
    }
    fclose(fp); // close the file

    Coordinate sorted_coordinates[NUM_SETS][1000/NUM_SETS];

    int sizes[NUM_SETS] = {0}; // Initialize the sizes array to all 0s
    /* The sizes array is used to keep track of how many coordinates 
    have been assigned to each row (i.e., for each z-coordinate value) */

    for (i = 0; i < 1000; i++) {//iterates through all 100 coordinates and places them in the appropriate set in the sorted_coordinates
        sorted_coordinates[coordinates[i].z][sizes[coordinates[i].z]] = coordinates[i]; //feeds entire x,y,z
        /* 
        coordinates[i].z represents the z-coordinate of the i-th coordinate in the coordinates array,
        row index is determined by its z-coordinate value
        sizes[coordinates[i].z] will output the current number of coordinates in the sorted_coordinates array that have the same z
        It keeps track of the size of each set of coordinates,
        so that the sorted_coordinates array can be properly sorted for each set of z coordinate values
        places the ith coordinate in the array at the appropriate index based on its z-coordinate value 
        */

        sizes[coordinates[i].z]++;
        // increments the size of the set that the coordinate was placed in
    }
    for (i = 0; i < NUM_SETS; i++) {
        qsort(sorted_coordinates[i], sizes[i], sizeof(Coordinate), compare_coordinates);
    }
    // write the sorted coordinates to separate files for each z-coordinate set
 
    fp = fopen("coordinates_update.xyz", "w"); // open the file for writing
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int j;
    for (i = 0; i < NUM_SETS; i++) {
        for (j = 0; j < sizes[i]; j++) {
            fprintf(fp, "%d %d %d\n", sorted_coordinates[i][j].x, sorted_coordinates[i][j].y, i);
        }
        if (i != NUM_SETS - 1) {
            fprintf(fp, "\n"); // add a newline after each set except the last one
        }
    }
    fclose(fp); // close the file

    return 0;
}
