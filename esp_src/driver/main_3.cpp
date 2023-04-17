#include <Arduino.h>


#define LAYER_DATA_PIN 4
#define LAYER_CLOCK_PIN 5
#define ROW_DATA_PIN 6
#define ROW_CLOCK_PIN 7
#define LAYER_ENABLE_PIN 8
#define ROW_ENABLE_PIN 9
#define COLUMN_ENABLE_PINS {10, 11, 12, 13, 14, 15, 16, 17}

// Buffer arrays from generator.c code?
uint8_t buffer[8][8][8]; //[layer][row][column]

// Initialize shift registers and other hardware components
void setup() {
  pinMode(LAYER_DATA_PIN, OUTPUT);
  pinMode(LAYER_CLOCK_PIN, OUTPUT);
  pinMode(ROW_DATA_PIN, OUTPUT);
  pinMode(ROW_CLOCK_PIN, OUTPUT);
  pinMode(LAYER_ENABLE_PIN, OUTPUT);
  digitalWrite(LAYER_ENABLE_PIN, LOW);
  pinMode(ROW_ENABLE_PIN, OUTPUT);
  digitalWrite(ROW_ENABLE_PIN, LOW);
  for (int i = 0; i < 8; i++) {
    pinMode(COLUMN_ENABLE_PINS[i], OUTPUT);
    digitalWrite(COLUMN_ENABLE_PINS[i], LOW);
  }
}

// Shift data into shift registers for a single layer


/*shiftColumnDat() and shiftLayerData() are functions that are responsible for shifting data 
into the shift registers for the columns and layers of the cube, respectively. 
These functions shift in the appropriate data for each column or layer and then use the appropriate clock signal*/

/* to prepare the shift registers to provide power and ground signals to the LEDs,
 by shifting in the appropriate data for each column and layer. */

void shiftLayerData(uint8_t layerIndex) {
  digitalWrite(LAYER_ENABLE_PIN, LOW);
  for (int i = 0; i < 8; i++) {
    shiftOut(LAYER_DATA_PIN, LAYER_CLOCK_PIN, MSBFIRST, buffer[layerIndex][i]);
    // The shiftOut function is used to shift the data for a --single layer-- into the shift register for that layer.
    /*buffer[layerIndex][i] is the data for the current layer and column being processed, 
    where i is the index of the column within the layer */

    /*buffer[layerIndex==0][i] refers to a 
    one-dimensional array of length 8 that contains the data for the LEDs in column i and layer 0.*/

    //buffer[layerIndex==0][i] refers to the ith row of the first layer in the buffer array. 
    //It is a one-dimensional array with 8 elements, each element representing the state of one LED in that row.

  }
  digitalWrite(LAYER_ENABLE_PIN, HIGH);
}



// Shift data into shift registers for a single column
void shiftColumnData(uint8_t columnIndex) {
  digitalWrite(ROW_ENABLE_PIN, LOW);
  for (int i = 0; i < 8; i++) {
    shiftOut(ROW_DATA_PIN, ROW_CLOCK_PIN, MSBFIRST, buffer[i][columnIndex]);
    /* to load the data for all columns, you would need to call the function shiftColumnData() 
    for each column, iterating through columnIndex from 0 to 7 */
  }
  digitalWrite(ROW_ENABLE_PIN, HIGH);
}

// Light up a single LED at the given row, column, and layer
void lightUpLED(uint8_t row, uint8_t column, uint8_t layer) {
  digitalWrite(LAYER_ENABLE_PIN, LOW);
  shiftOut(LAYER_DATA_PIN, LAYER_CLOCK_PIN, MSBFIRST, 1 << layer);
  digitalWrite(LAYER_ENABLE_PIN, HIGH);
  digitalWrite(ROW_ENABLE_PIN, LOW);
  shiftOut(ROW_DATA_PIN, ROW_CLOCK_PIN, MSBFIRST, 1 << row);
  digitalWrite(ROW_ENABLE_PIN, HIGH);
  digitalWrite(COLUMN_ENABLE_PINS[column], HIGH);
}





//ask why uint8_t is used for the argument

// Iterate through all LEDs in the cube and light them up in sequence
void lightUpCube() {
  for (int layer = 0; layer < 8; layer++) {
    shiftLayerData(layer);
    for (int column = 0; column < 8; column++) {
      shiftColumnData(column);
      for (int row = 0; row < 8; row++) {
        lightUpLED(row, column, layer);
        delay(1); // Adjust delay as necessary for your specific hardware
      }
      digitalWrite(COLUMN_ENABLE_PINS[column], LOW);
    }
  }
}

void loop() {
  // Call lightUpCube() to iterate through all LEDs in the cube and light them up in sequence
  lightUpCube();
}
