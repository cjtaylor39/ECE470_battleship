//Functions for SPI communication between battleships

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "spi.h"

//configureSPIm--------------------------------------------------------------------
// Configures SPI0 as master
// Arguments:	none
// Return:		none
void configureSPIm(void) {
  DDRS |= 0xE0; 		// SS, SCLK, MOSI outputs
  DDRS &= 0xEF; 		//MISO as input
  SPI0BR = BAUD_375000; //Configure baud to 375000 = 24M/((7+1)*2^(2+1))
  SPI0CR1 = 0b01010011;
  SPI0CR2 = 0x10; 		//Automatically asserts CS when writing
  //WOMS = 0x00;  		//enable pullups for PORTS
}

//configureSPIs--------------------------------------------------------------------			Could combine both configure statements to accept 'm' or 's'
// Configures SPI0 as slave
// Arguments:	none
// Return:		none
void configureSPIs(void) {
  DDRS &= 0x1F; 		//SS, SCLK, MOSI inputs
  DDRS |= 0x10; 		//MISO output
  SPI0BR = BAUD_375000; //Not sure if this is necessary in the slave
  SPI0CR1 = 0b01000001;
  SPI0CR2 = 0x00; 		//No bidirectional support
}

//sendByteSPI--------------------------------------------------------------------
// Sends char across SPI0 communication
// Arguments:	data to be sent across SPI0
// Return:		none
void sendByteSPI(char bite) {
  while ((SPI0SR & 0x20) == 0); //Check and wait for the transmission flag != 0
  SPI0DR = bite;
}

//receiveByteSPI--------------------------------------------------------------------
// Returns data from SPI0 communication
// Arguments: 	none
// Return: 		Char from other SPI device
char receiveByteSPI(void) {
  while (!(SPI0SR & 0x80)); //Checks the SPIF flag
  return SPI0DR;  
}