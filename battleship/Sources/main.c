#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"
#include "pixels.h"
#include "spi.h"

#define DUMMY 0x05

void transition(void){
  int i = 0;
  int j = 0;
  int off = 0;
  for (j = 0; j < 64; j++) {
    for (i = 0; i < off; i++) {
      sendPixel(0,0,0);
      j++;
    }
    sendPixel(0,0,10);
    off++;
  }
  msDelay(50);
}

//Belongs in util.h?
typedef enum state_type {
  ATTACK,
  DEFEND  
} state_type;

void main(void){
  char matrix[8][8] = {1,1,1,1,1,1,1,1,
                       3,3,3,3,3,3,3,3,
                       3,3,3,3,3,3,3,3,
                       3,3,3,3,3,3,3,3,
                       1,1,1,1,1,1,1,1,
                       1,1,1,1,1,1,1,1,
                       1,1,1,1,1,1,1,1,
                       3,3,3,3,3,3,3,3};

  state_type state = ATTACK; 
  unsigned char dataIn = 0;
  unsigned char dataOut = 0;
  enableLEDs();
  DDRH = 0x00;
  configureSPIm();
  
  for(;;){
    // Hit/Miss demo  (Master)
    while((PTH & 0x01) != 0); //Push SW5 to send code from DIPs
      sendByteSPI(PTH);
      dataIn = receiveByteSPI();      //Store whatever the slave just sent us (remember we must initialize the slaves SPI0DR to hold something useuful)
      PORTB = dataIn;
      
      //dataIn = 0xFF;
      msDelay(500);   //debounce
      //Something is wrong with this loop, never enters here and somehow I recieve 0xFE, as they are displayed on the LEDs.
      while(dataIn == 0xFF) {   //Waiting for P2 to send hit/miss
        sendByte(0x05);
        dataIn = receiveByteSPI();  //This 
        msDelay(500); //Wait half second for P2 to compute hit/miss
      }
      /*
      if (dataIn == 0x02) {
        PORTB = 0xFF; //Hit 
      }
      else if (dataIn == 0x03) {
        PORTB = 0xFE; //Miss
      }
      else {
        PORTB = 0x55; //Transmission error
      }
      */
    }
}


 