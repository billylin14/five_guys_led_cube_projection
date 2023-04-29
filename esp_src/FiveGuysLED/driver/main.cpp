#include <Arduino.h>
#include <stdlib.h>
#include <unistd.h>

#define Layer 4
#define Row1 13
#define Row2 12
#define Row3 27
#define Layer1 33
#define Layer2 15
#define Layer3 32
#define Row7 14
#define Row8 22
#define row_clk 21
#define layer_clk 25

#define NUM_SETS 8

#define ROWS 8
#define COLS 8
#define BUFFER_SIZE ROWS * COLS

int layer_flag = 8;
int row_flag;
bool buffer[8][8]; //buffer[row][col];


int sizes[NUM_SETS] = {0}; // Initialize the sizes array to all 0s

typedef struct {
    int x;
    int y;
    int z;
} Coordinate;
//FUNCTIONS FOR SORTING
int compare_coordinates(const void *a, const void *b) {
    Coordinate *c1 = (Coordinate*)a;
    Coordinate *c2 = (Coordinate*)b;
    return c1->z - c2->z;
}


//FUNCTIONS FOR LIGHTING LEDS
//To be deleted
void temp_buffer_creation(){
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      buffer[i][j] == 1;
    }
  }
  buffer[2][5] == 0;
  buffer[7][3] == 0;
  buffer[6][3] == 0;
  buffer[0][4] == 0;
}
// this function reads the serial for xyz coordinates
void read_serial_coor(void * parameter){

}

//this function takes the serial data and sorts it by z values


//this function takes each row of data and pushes 8 bits of data into the shift registers
void layer_interrupt() {
  for (row_flag=0; row_flag<8; row_flag++){
   
    // data is shifted on rising edge
    digitalWrite(row_clk, 0);
    digitalWrite(Row1, buffer[row_flag][0]); //flash [row_flag][1st column] bit each digital wrte is 3.4E-03
    digitalWrite(Row2, buffer[row_flag][1]); //flash [row_flag][2st column] bit
    digitalWrite(Row3, buffer[row_flag][2]); //flash [row_flag][3st column] bit
    digitalWrite(Row4, buffer[row_flag][3]); //flash [row_flag][4st column] bit
    digitalWrite(Row5, buffer[row_flag][4]); //flash [row_flag][5st column] bit
    digitalWrite(Row6, buffer[row_flag][5]); //flash [row_flag][6st column] bit
    digitalWrite(Row7, buffer[row_flag][6]); //flash [row_flag][7st column] bit
    digitalWrite(Row8, buffer[row_flag][7]); //flash [row_flag][8st column] bit
    digitalWrite(row_clk, 1);

    // faster write
    
    }

}

//this function calls layer_interrupt and lights up layer *HAS TASKDELAY 15ms
void readlight_layer(void * parameter){
  while(true){ 
    // read from buffer array

    //Flash data for new layer
    layer_interrupt();

    //Tell 8 bit shift register to light new layer ********** //
    digitalWrite(layer_clk,0);                                //
    //check if flag is past 8 layers--> reset layer count     //
    if(layer_flag != 8) {                                     //
      layer_flag = 0;                                         //
      digitalWrite(layer_flag, 0);                            //
    } else{                                                   //
      digitalWrite(layer_flag, 1);                            //
    }                                                         //
    digitalWrite(layer_clk, 1);                               //
    //********************************************************//

    //update layer flag for upcoming layer
      layer_flag++;

    vTaskDelay(15 / portTICK_PERIOD_MS); // PAUSES TASK for 15 ms to run data processing tasks
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Layer, OUTPUT);
  pinMode(Row1, OUTPUT);
  pinMode(Row2, OUTPUT);
  pinMode(Row3, OUTPUT);
  pinMode(Row4, OUTPUT);
  pinMode(Row5, OUTPUT);
  pinMode(Row6, OUTPUT);
  pinMode(Row7, OUTPUT);
  pinMode(Row8, OUTPUT);
  digitalWrite(layer_clk, 0);
  digitalWrite(row_clk, 0);

//temp buffer creation//
temp_buffer_creation();


  xTaskCreate(readlight_layer, "Light LED layer", 1000, NULL, 1, NULL );
}

void loop() {
//reads one layer of buffer, input into 8x8x8 buffer cube
  
}