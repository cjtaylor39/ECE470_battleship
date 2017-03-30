//Header file containing prototypes and globals for adafruit neopixel 8x8 matrix

//LCD Macros
#define SWI   5

//sendPixel--------------------------------------------------------------------
// Turns specific neopixel specified color
// Arguments:	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:	 	none
extern void sendPixel(unsigned char r, unsigned char g, unsigned char b);

//display--------------------------------------------------------------------
// Turns specific neopixel specified color
// Arguments: 	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:		none
extern void display(char r, char g, char b);

//HAVENT TESTED USING THE POINTER TO PASS IN BOARD YET
//displayBoard--------------------------------------------------------------------
// Displays battleship board based on global "board"
// Arguments:	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:		none
extern interrupt SWI void displayBoard(char** board);

//ASSEMBLY FUNCTION

//sendByte--------------------------------------------------------------------
// Sends 
// Arguments:	int number of ms to be delayed
// Return:		none
extern void sendByte(char);