#include <Arduino.h>

#define Layer 4
#define Row1 13
#define Row2 12
#define Row3 27
#define Row4 33
#define Row5 15
#define Row6 32
#define Row7 14
#define Row8 22
#define row_clk 21
#define layer_clk 25
#define ROW_SIZE 8
#define LAYER_SIZE 8
#define SetBitOn(var, bit) (var |= (1 << bit)) //Set bit in byte to 1 function !temporary
#define SetBitOff(var, bit) (var &= ~(1 << bit)) //Set bit in byte to 0 function !temporary
#define GetBit(var, bit) ((var & (1 << bit)) != 0) //Get bit in byte function

typedef struct BitLayerBuff{
  uint8_t buff[ROW_SIZE];
  SemaphoreHandle_t mutex;
} BitLayerBuff;


uint8_t row_flag;
uint8_t layer_flag;
static BitLayerBuff bitBufs[LAYER_SIZE];

void temp_buffer_creation(){ //sets all bits in cube to 1
  for(int k=0; k<8; k++){
    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
        SetBitOn(bitBufs[k].buff[i],j);
      }
    }
  }

  SetBitOff(bitBufs[2].buff[4],2); //sets bit layer 3, row 5, column 3 to 0
}

// Initialize layer_flag to 0
// Initialize layer_clk and row_clk to 0

void layer_write() {
  for (row_flag=0; row_flag<8; row_flag++){
    // data is shifted on rising edge
    digitalWrite(row_clk, 0);
    digitalWrite(Row1, GetBit(bitBufs[layer_flag].buff[row_flag],0)); //flash [row_flag][1st column] bit each digital wrte is 3.4E-03
    digitalWrite(Row2, GetBit(bitBufs[layer_flag].buff[row_flag],1)); //flash [row_flag][2st column] bit
    digitalWrite(Row3, GetBit(bitBufs[layer_flag].buff[row_flag],2)); //flash [row_flag][3st column] bit
    digitalWrite(Row4, GetBit(bitBufs[layer_flag].buff[row_flag],3)); //flash [row_flag][3st column] bit
    digitalWrite(Row5, GetBit(bitBufs[layer_flag].buff[row_flag],4)); //flash [row_flag][3st column] bit
    digitalWrite(Row6, GetBit(bitBufs[layer_flag].buff[row_flag],5)); //flash [row_flag][3st column] bit
    digitalWrite(Row7, GetBit(bitBufs[layer_flag].buff[row_flag],6)); //flash [row_flag][3st column] bit
    digitalWrite(Row8, GetBit(bitBufs[layer_flag].buff[row_flag],7)); //flash [row_flag][3st column] bit

    digitalWrite(row_clk, 1);
    }
}

//this function calls layer_interrupt and lights up layer *HAS TASKDELAY 15ms
void readlight_layer(){
    layer_write();

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
    //Set the function to delay for -- milliseconds
}