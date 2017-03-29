//Header file containing prototypes and globals for SPI communication

//SPI Macros
#define CS          PTS_PTS7
#define BAUD_375000 0x72  //375000 = 24M/((7+1)*2^(2+1))

//configureSPIm--------------------------------------------------------------------
// Configures SPI0 as master
// Arguments:	none
// Return:		none
extern void configureSPIm(void);

//configureSPIs--------------------------------------------------------------------			Could combine both configure statements to accept 'm' or 's'
// Configures SPI0 as slave
// Arguments:	none
// Return:		none
extern void configureSPIs(void);

//sendByteSPI--------------------------------------------------------------------
// Sends char across SPI0 communication
// Arguments:	data to be sent across SPI0
// Return:		none
extern void sendByteSPI(char bite);

//receiveByteSPI--------------------------------------------------------------------
// Returns data from SPI0 communication
// Arguments: 	none
// Return: 		Char from other SPI device
extern char receiveByteSPI(void);