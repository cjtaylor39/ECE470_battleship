//Header file containing prototypes and globals for SPI communication

//SPI Macros
#define HITMSG  0x02
#define MISSMSG 0x03

//configureSCI--------------------------------------------------------------------
// Configures SCI0 channel for serial communication
// Arguments:	none
// Return:		none
extern void configureSCI(void);

//sendByteSCI--------------------------------------------------------------------
// Sends char across SPI0 communication
// Arguments:	data to be sent across SPI0
// Return:		none
extern void sendByteSCI(char bite);

//receiveByteSCI--------------------------------------------------------------------
// Returns data from SPI0 communication
// Arguments: 	none
// Return: 		Char from other SPI device
extern char receiveByteSCI(void);