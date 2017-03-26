#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"

//Macros for LED stuff
#define UP    1
#define DOWN  0

//Macros for SPI stuff
#define CS          PTS_PTS7
#define BAUD_375000 0x72  //375000 = 24M/((7+1)*2^(2+1))

//prototypes
void LED_enable(void);
void msDelay(int);
/* Not sure if protoypes for C functions are necessary
void display(char r, char g, char b);
void rainbow(void);
void gradient(char color, int upDown, char r, char g, char b);
void displayBoard(void);
void sendPixel(unsigned char r,unsigned char b,unsigned char g);
*/
void sendByte(char);

char board[64] = {0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,
                  1,1,1,1,1,1,1,1,
                  1,1,1,1,1,1,1,1,
                  2,2,2,2,2,2,2,2,
                  3,3,3,3,3,3,3,3};
//Use SPI0 as master  
void configureSPIm(void) {
  DDRS |= 0xE0; // SS, SCLK, MOSI outputs
  DDRS &= 0xEF; //MISO as input
  SPI0BR = BAUD_375000; //Configure baud to 375000 = 24M/((7+1)*2^(2+1))
  SPI0CR1 = 0b01010011;
  SPI0CR2 = 0x10; //Automatically asserts CS when writing
  //WOMS = 0x00;  //enable pullups for PORTS
}

void SPIpause(void) {            //Provides 21.59us delay for CS when used in sendByteSPI0
  int i = 0;
  for(i = 185; i != 0; i--) {    
    }  
}

void sendByteSPI(char bite) {
  while ((SPI0SR & 0x20) == 0); //Check and wait for the transmission flag != 0
  //CS = 0;                 //Pull CS low for start of comm
  SPI0DR = bite;
  //SPIpause();             //Need delay here for message transmission time for CS to be pulled low
  //CS = 1;                 //Bring CS back up for end of comm
}

//Using SPI1 to recieve as slave
void configureSPIs(void) {
  DDRS &= 0x1F;  //SS, SCLK, MOSI inputs
  SPI0BR = BAUD_375000; //Not sure if this is necessary in the slave
  SPI1CR1 = 0b01000101; //enable interrupt, enable SPI, enable SPTEF?, slave, active low clk, rising edge sampling,enable CS output,transfer data LSB first   
  SPI1CR2 = 0x00; //No bidirectional support
  //WOMS = 0x00;  //enable pullups for PORTS
}

char receiveByteSPI(void) {
  while (!(SPI0SR & 0x80)); //Checks the SPIF flag
  return SPI0DR;  
}


void sendPixel(unsigned char r, unsigned char g, unsigned char b) {
  sendByte(g);
  sendByte(r);
  sendByte(b); 
}

void display(char r, char g, char b) {
  int i = 0;
  for(i = 0; i<64;i++) {
    sendPixel(r,g,b);    
  }
  msDelay(10);  
}

void gradient(char color, int upDown, char r, char g, char b){  //needs to return r,g,b or make them global :p
  int i = 0;
  if (upDown == UP) {
    for(i = 0; i<50; i++){
      if (color == 'r') {
        r++;
      } 
      else if (color == 'g') {
        g++;  
      }
      else {
        b++;  
      }
      display(r,g,b);  
    }
  }
  else {
    for(i = 0; i<50; i++){
      if (color == 'r') {
        r--;
      } 
      else if (color == 'g') {
        g--;  
      }
      else {
        b--;  
      }
      display(r,g,b);   
    }
  }
}   


void rainbow(void)  {
  int i = 0;
  char r = 0;
  char g = 0;
  char b = 0;
  display(r,g,b); //start by turned off
  gradient('r',UP,r,g,b);
  gradient('g',UP,r,g,b);
  gradient('r',DOWN,r,g,b);
  gradient('b',UP,r,g,b);
  gradient('g',DOWN,r,g,b);
  gradient('r',UP,r,g,b);
  gradient('b',DOWN,r,g,b);
}

void displayBoard(void) {
  //Interrupt start here cli();
  int i = 0;
  for(i = 0; i<64;i++) {
    if (board[i] == 0) {
      sendPixel(0,0,10);    //water space
    }
    else if(board[i] == 1) {
      sendPixel(0,10,10);  //boat space  
    }
    else if(board[i] == 2) {
      sendPixel(0,5,10);    //miss space
    }
    else
      sendPixel(0,2,10);    //hit space  
  }
  msDelay(5);
}

void main(void){  //might remove if only 1 displaying function
  char data = 0x01; //data to be sent out
  LED_enable();
  DDRH = 0x00;
  configureSPIm();
  
  for(;;){ 
    while((PTH & 0x01) != 0); //Push SW5 to start a transmission
      sendByteSPI(data);
      data = receiveByteSPI();      //Store whatever the slave just sent us (remember we must initialize the slaves SPI0DR to hold something useuful)
      PORTB = data;
      data++;
      msDelay(500);  //debounce
    }
}


 