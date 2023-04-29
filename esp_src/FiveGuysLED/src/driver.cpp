#include <Arduino.h>
#include <driver.h>




uint8_t row_flag;


//timing
long int t1;
long int t2;

#if TEST

//temp functions
#define SetBitOn(var, bit) (var |= (1 << bit)) //Set bit in byte to 1 function !temporary
#define SetBitOff(var, bit) (var &= ~(1 << bit)) //Set bit in byte to 0 function !temporary

//put in main dont define as static
static BitLayerBuff bitBufs[LAYERS];

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
#endif

// Initialize layer_flag to 0
// Initialize layer_clk and row_clk to 0

void layer_write(BitLayerBuff bitBufs) {
  for (row_flag=0; row_flag<8; row_flag++){
    // data is shifted on rising edge
    digitalWrite(row_clk, 0);
    //simplify by using for loop
    digitalWrite(Row1, GetBit(bitBufs.buff[row_flag],0)); //flash [row_flag][1st column] bit each digital wrte is 3.4E-03
    digitalWrite(Row2, GetBit(bitBufs.buff[row_flag],1)); //flash [row_flag][2st column] bit
    digitalWrite(Row3, GetBit(bitBufs.buff[row_flag],2)); //flash [row_flag][3st column] bit
    digitalWrite(Row4, GetBit(bitBufs.buff[row_flag],3)); //flash [row_flag][3st column] bit
    digitalWrite(Row5, GetBit(bitBufs.buff[row_flag],4)); //flash [row_flag][3st column] bit
    digitalWrite(Row6, GetBit(bitBufs.buff[row_flag],5)); //flash [row_flag][3st column] bit
    digitalWrite(Row7, GetBit(bitBufs.buff[row_flag],6)); //flash [row_flag][3st column] bit
    digitalWrite(Row8, GetBit(bitBufs.buff[row_flag],7)); //flash [row_flag][3st column] bit

    digitalWrite(row_clk, 1);
    }
}

/*void setup(){
temp_buffer_creation();
Serial.begin(115200);
pinMode(Layer, OUTPUT);
pinMode(Row1, OUTPUT);
pinMode(Row2, OUTPUT);
pinMode(Row3, OUTPUT);
pinMode(Row4, OUTPUT);
pinMode(Row5, OUTPUT);
pinMode(Row6, OUTPUT);
pinMode(Row7, OUTPUT);
pinMode(Row8, OUTPUT);
pinMode(row_clk, OUTPUT);
pinMode(layer_clk, OUTPUT);

}*/

//timing test 
/*
void loop(){
t1 = millis();
readlight_layer();
t2 = millis();
Serial.print(t2-t1); Serial.println("milliseconds");
} */