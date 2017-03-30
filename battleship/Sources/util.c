//Misc functions for battleship game

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"
#include "mc9s12dg256.h"

//enableLEDs--------------------------------------------------------------------
// Initialize LEDs as display on dragon board
// Arguments: 	none
// Output:		none
void enableLEDs(void){
	asm {
  	BSET DDRJ, #$02   ; config PJ1 as output pin
    BCLR PTJ,  #$02   ; drive PJ1 low
            
    MOVB #$FF, DDRB   ; config PB7-PB0 as output pins
    CLR PORTB         ; starts off
  	RTS
	}
}

//msDelay--------------------------------------------------------------------
// Creates delay of specified milliseconds
// Arguments:	number of ms to be delayed
// Output: 		none
void msDelay(int ms) {
	asm {
		msDelay:                                    
			    PSHX              ;2  clock cycles
		Outer: 
          LDX  #7995        ;2     
		Inner: 
          DBNE X, Inner     ;3/3    ( 3 cycles both ways)
	          
	        DBNE D, Outer     ;3/3
          PULX              ;3 
 	        RTS               ;5 
	}
}


//clockSet--------------------------------------------------------------------
// Sets up PPL to achieve 24Mhz always
// Arguments:	none
// Output: 		none
void clockSet(void) {
	CLKSEL &= 0x7F;	//Use OCLK now
	PLLCTL |= 0x60;
	SYNR = 0x05;
	REFDV = 0x01;	//PLLCLK = 2 * OCLK(4MHZ) * {SYNR + 1}(6)/{REDV + 1}(1) = 24Mhz
	while(!(0x08 & CRGFLG));   //wait until PLL locks to the desired frequency
	CLKSEL |= 0x80;	//Use PLL now
}