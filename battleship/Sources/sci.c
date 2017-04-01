//Functions for SPI communication between battleships

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci.h"

//configureSCI--------------------------------------------------------------------
// Configures SPI0 as master
// Arguments:	none
// Return:		none
void configureSCI(void) {
  DDRS |= 0x02; //TX pin output
  DDRS &= 0xFE; //RX pin input
  SCI0BDH = 0x00; //Gotta change this baud rate
  SCI0BDL = 0x0D;
  SCI0CR1 = 0x00;
  SCI0CR2 = 0x0C; //Transmit an Recieve enabled, no interrupts  
}

//sendByteSCI--------------------------------------------------------------------
// Sends byte across serial TX line
// Arguments:	none
// Return:		none
void sendByteSCI(char data) {
  while(!(SCI0SR1 & 0x80));
  SCI0DRL = data; 
}

//receiveByteSCI--------------------------------------------------------------------
// Recieves and returns byte from RX line
// Arguments:	none
// Return:		none
char receiveByteSCI(void){
  while(!(SCI0SR1 & 0x20));
  return SCI0DRL;  
}