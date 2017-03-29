//Functions for controlling 8x8 neopixel matrix for battleship game

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "pixels.h"

//sendPixel--------------------------------------------------------------------
// Turns specific neopixel specified color
// Arguments:	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:	 	none
void sendPixel(unsigned char r, unsigned char g, unsigned char b) {
  sendByte(g);
  sendByte(r);
  sendByte(b); 
}
//display--------------------------------------------------------------------
// Turns specific neopixel specified color
// Arguments: 	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:		none
void display(char r, char g, char b) {
  int i = 0;
  for(i = 0; i<64;i++) {
    sendPixel(r,g,b);    
  }
  msDelay(10);  
}

//MIGHT NEED TO CHANGE FROM GLOBAL TO POINTER, FOR MULTIPLE BOARD DISPLAY
//displayBoard--------------------------------------------------------------------
// Displays battleship board based on global "board"
// Arguments:	8bit value for red, 8bit value for green, 8bit value for blue,
// Return:		none
interrupt SWI void displayBoard(void) {
  int i = 0;
  int j = 0;
  for(i = 0; i<8;i++) {
    for(j = 0; j < 8; j++) {
      if (board[i][j] == 0) {
        sendPixel(0,0,10);    //water space
      }
      else if(board[i][j] == 1) {
        sendPixel(0,0,10);  //boat space  
      }
      else if(board[i][j] == 3) {
        sendPixel(0,4,10);    //miss space
      }
      else
        sendPixel(0,2,10);    //hit space  
    }
  }
  msDelay(5);
}

//ASSEMBLY FUNCTION

//sendByte--------------------------------------------------------------------
// Sends 
// Arguments:	number of ms to be delayed
// Return:		none
void sendByte(char){
	asm {
		sendByte:         		;B contains the char to be sent out serially
              LDAA #08    		;Count of bits
      next:   CMPB #00    		;Bring B back in focuse
              BMI send1     	;Check if MSB is 0 or 1
              BRA send0
   
      back:   LSLB
              DECA        
              BNE next
              RTS

      send1:  BSET PORTB, #01          
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP
              NOP             	;High pulse of 706ns
              BCLR PORTB, #01
              BRA back             
              
      send0:  BSET PORTB, #01
              NOP
              NOP
              NOP
              NOP
              NOP              ;High pulse of 375ns
              BCLR PORTB, #01
              BRA back                         
              
              ;After RTS and assuming we are entering another
              ;senbit() we have a minimum of 669ns low pulse which
              ;meets the timing requirement between bits
	}
}