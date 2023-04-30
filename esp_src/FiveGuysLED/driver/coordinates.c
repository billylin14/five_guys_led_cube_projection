//Arthor: Junwon Hwang & Michael Wang
//If any code changes, please contact --both-- of us. 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define num_coord 1000

int main() {
    srand(time(NULL)); // seed the random number generator with current time
    FILE *fp = fopen("coordinates.xyz", "w"); // open the file for writing
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    int i;
    for (i = 0; i < num_coord; i++) {
        int x = rand() % 8; // generate random x coordinate between 0 and 7
        int y = rand() % 8; // generate random y coordinate between 0 and 7
        int z = rand() % 8; // generate random z coordinate between 0 and 7
        fprintf(fp, "%d %d %d\n", x, y, z); // write the coordinates to the file
    }
    fclose(fp); // close the file
    return 0;
}
