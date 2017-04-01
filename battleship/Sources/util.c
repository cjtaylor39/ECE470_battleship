//Misc functions for battleship game

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"
#include <stdlib.h>
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

//msDelay----------------------------------------------------------------------
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


//clockSet----------------------------------------------------------------------
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

//Keypad Function-------------------------------------------------------------

char hexKeypad(void){                          //OPEN MAIN
   
  const unsigned char keypad_ascii[4][4] =
  {
  1,2,3,0,
  4,5,6,0,
  7,8,9,0,
  0,0,0,0
  };
  unsigned char column,row;
  
   char val = 0;
   DDRB = 0xFF;                           //MAKE PORTB OUTPUT
   DDRJ |=0x02; 
   PTJ &=~0x02;                            //ACTIVATE LED ARRAY ON PORT B
   DDRP |=0x0F;                           //
   PTP |=0x0F;                            //TURN OFF 7SEG LED
   DDRA = 0x0F;                           //MAKE ROWS INPUT AND COLUMNS OUTPUT
   
 
   while(1){                              //OPEN WHILE(1)
      do{                                 //OPEN do1
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1



      do{                                 //OPEN do2
         do{                              //OPEN do3
            msDelay(1);                   //WAIT
            row = PORTA & 0xF0;           //READ ROWS
         }while(row == 0x00);             //CHECK FOR KEY PRESS //CLOSE do3
         
         msDelay(15);                     //WAIT FOR DEBOUNCE
         row = PORTA & 0xF0;
      }while(row == 0x00);                //FALSE KEY PRESS //CLOSE do2

      while(1){                           //OPEN while(1)
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x01;                   //COLUMN 0 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 0
            column = 0;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x02;                   //COLUMN 1 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 1
            column = 1;
            break;                        //BREAK OUT OF while(1)
         }

         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x04;                   //COLUMN 2 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 2
            column = 2;
            break;                        //BREAK OUT OF while(1)
         }

         row = 0;                         //KEY NOT FOUND
      break;                              //step out of while(1) loop to not get stuck
      }                                   //end while(1)

      if(row == 0x10){
         val = keypad_ascii[0][column];         //OUTPUT TO PORTB LED
      //   return keypad[0][column];
 
      }
      else if(row == 0x20){
         val = keypad_ascii[1][column];
     //   return keypad[0][column];
 
      }
      else if(row == 0x40){
         val=keypad_ascii[2][column];
     // return keypad[0][column];
 
      }

      do{
         msDelay(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS NOT STILL HELD
      return val;
   }                                      //CLOSE WHILE(1)
}                                         //CLOSE MAIN

// LCD Functions ---------------------------------------------------------------

//Write command to the LCD
void COMWRT4(unsigned char command)
  {
        unsigned char x;
        
        x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;          //clear bits Pk5-Pk2
        LCD_DATA = LCD_DATA | x;          //sends high nibble to PORTK
        msDelay(1);
        LCD_CTRL = LCD_CTRL & ~RS;         //set RS to command (RS=0)
        msDelay(1);
        LCD_CTRL = LCD_CTRL | EN;          //raise enable
        msDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
        msDelay(15);                       //wait
        x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;         //clear bits Pk5-Pk2
        LCD_DATA =LCD_DATA | x;             //send low nibble to PORTK
        LCD_CTRL = LCD_CTRL | EN;          //raise enable
        msDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //drop enable to capture command
        msDelay(15);
  }

//Write data to the LCD
void DATWRT4(unsigned char data)
  {
  unsigned char x;
         
        
        x = (data & 0xF0) >> 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        msDelay(1);
        LCD_CTRL = LCD_CTRL | RS;
        msDelay(1);
        LCD_CTRL = LCD_CTRL | EN;
        msDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        msDelay(5);
       
        x = (data & 0x0F)<< 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        LCD_CTRL = LCD_CTRL | EN;
        msDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        msDelay(15);
  }

//Initialize the LCD  
void lcdInit(void){

        DDRK = 0xFF;   
        COMWRT4(0x33);   //reset sequence provided by data sheet
        msDelay(1);
        COMWRT4(0x32);   //reset sequence provided by data sheet
        msDelay(1);
        COMWRT4(0x28);   //Function set to four bit data length
                                         //2 line, 5 x 7 dot format
        msDelay(1);
        COMWRT4(0x06);  //entry mode set, increment, no shift
        msDelay(1);
        COMWRT4(0x0E);  //Display set, disp on, cursor on, blink off
        msDelay(1);
        COMWRT4(0x01);  //Clear display
        msDelay(1);
        COMWRT4(0x80);  //set start posistion, home position
        msDelay(1);


}

//Display predefined messages 1-7 on the LCD
void dispLCD(unsigned char option){
 
 
 
  //Player 1 turn messages
        char p1Turn1[] = "Your turn!";
        char* p1Turn1ptr = &p1Turn1;
        char p1Turn2[] = "Enemy Ships: ";
        char* p1Turn2ptr = &p1Turn2;
        
        char p1Hit[] = "You hit a ship!";
        char* p1Hitptr = &p1Hit;
        
        char p1Miss[] = "You missed!";
        char* p1Missptr = &p1Miss;
        
        char p1Win[] = "You win!";
        char* p1Winptr = &p1Win;
        
        char p1Lose[] = "You lose!";
        char* p1Loseptr = &p1Lose;
        
  //player 2 turn messages        
        char p2Hit1[] = "Your ship was";
        char* p2Hit1ptr = &p2Hit1;
        char p2Hit2[] = "hit!";
        char* p2Hit2ptr = &p2Hit2;
        
        char p2Turn1[] = "Enemy turn!";
        char* p2Turn1ptr = &p2Turn1;
        
        char p2Turn2[] = "Your ships: ";
        char* p2Turn2ptr = &p2Turn2;

   COMWRT4(0x01);  //Clear display
 
    if(option == 1){  //Your turn!
      while(*p1Turn1ptr != 0){
        DATWRT4(*p1Turn1ptr);
        p1Turn1ptr++;
      }
      
    } else if(option == 2){ //You hit a ship!
        while(*p1Hitptr != 0){
          DATWRT4(*p1Hitptr);
        p1Hitptr++;
        }
       
     } else if(option == 3){ //You missed!
        while(*p1Missptr != 0){
          DATWRT4(*p1Missptr);
        p1Missptr++;
        }
        
     } else if(option == 4){ //You win!
        while(*p1Winptr != 0){
          DATWRT4(*p1Winptr);
        p1Winptr++;
        }
        
     } else if(option == 5){ //You lose!
        while(*p1Loseptr != 0){
          DATWRT4(*p1Loseptr);
        p1Loseptr++;
        }
        
     } else if(option == 6){ //Your ship was
        while(*p2Hit1ptr != 0){
          DATWRT4(*p2Hit1ptr);
        p2Hit1ptr++;
        }
        COMWRT4(0xC0);
        while(*p2Hit2ptr != 0){ //hit!
          DATWRT4(*p2Hit2ptr);
          p2Hit2ptr++;
        }
     } else if(option == 7){ //Enemy turn!
        while(*p2Turn1ptr != 0){
          DATWRT4(*p2Turn1ptr);
        p2Turn1ptr++;
        }
        
     }
}

int shipSum(char* ptr, int row, int col, char size, char orientation) {
	char sum=0;
	int k = 0;	
	if (orientation == 0) { 	              	//horizontal		
		for (k = col; k < col + size; k++) {
		   sum+=*(ptr+10*row+k);
		}
	}
	else {                                    // vertical
		for (k = row; k<row + size; k++) {
	    sum+=*(ptr+10*k+col);
		}
	}
	return sum;

}
                                      
void matrixBuild(char* ptr_large, char* ptr_small) {
	int row=0; int col=0;char ssum=1; 
	char i = 0;	int r = 0; int c = 0;
	char max = 7;   //random max number used for testing
	char shipsize[4] = {4,3,3,2};
  int orientation = 0;
  char keypress = 0;
  
  //------ to be implemented on the board with the LED grid!!------
  
  /*while(keypress !=5){ 
   r=rand();
   r+=r;
   keypress = hexKeypad();
   msDelay(750);
  }*/
  
  for (c=0;c<max;c++){
     	r=rand();
      r+=r;  
    }
	
	for (i = 0; i < 4; i++) {		
	           	                          
	    	orientation = rand();  //DO NOT ASK WHY ABOUT THE FOLLOWING TWO LINES	                  
        orientation +=2*orientation;
        orientation =orientation%2;

    	//---------Horizontal-----------------------------------------
    		if (orientation == 0) {
    		     ssum = 1;
          		while (ssum != 0) { 							//ensure ship in legal spot - not next to other ship
        				row = (rand()%8)+1;
        				col = rand()%(8 - shipsize[i]+1)+1;	            //ship has to fit n  to check if ship
        				ssum = shipSum(ptr_large, row, col, shipsize[i], orientation);
        			}         
        			
        	//----- pass this point it is allowed to place a ship
	          	for (r = row - 1; r < row + 2; r++) {
          				for (c = col - 1; c < col + shipsize[i] + 1; c++) {
  					
      				    	if ((r == row) & (c > col - 1) & (c < col + shipsize[i])) {
                    *(ptr_large+r*10+c)=1;
        						
        						}
        						else {
        						*(ptr_large+r*10+c)=9;
    						}
    					
  					}
				}
  }
    //---------VERTICAL----------------------------------------------
		else {	
		
          ssum = 1;
    			while (ssum != 0) { 							//ensure ship in legal spot - not next to other ship
    					        										// fixing if exceeded
    				col = rand()%8+1;
    				row = rand()%(8 - shipsize[i])+1;	            //ship has to fit n  to check if ship
    				ssum = shipSum(ptr_large, row, col, shipsize[i], orientation);
    			}         
    				//----- pass this point it is allowed to place a ship
			
          	for (r = row - 1; r < row + shipsize[i] + 1; r++) {
	          for (c = col - 1; c < col + 2; c++) {
  						if ((r > row-1) && (c == col) && (r < row + shipsize[i])) {
  						
  						  *(ptr_large+r*10+c)=1;
  						}
  						else {
  							*(ptr_large+r*10+c)=9;
  						}
  					
  				}
  			}
				
			}   
		}
		
		//CROP---------------------------------------
		//	converting large matrix into small one
  	for (r = 1; r < 9; r++) {
		//temp_cols = 0;
		for (c = 1; c < 9; c++) {
			
			*ptr_small=*(ptr_large+10*r+c) ;
			
			ptr_small++;
			//defense_board_p1[temp_rows][temp_cols] = defense_board_p1_large[p][q];
			//temp_cols ++;
		}
		//temp_rows++;       
	}  
		
	}  