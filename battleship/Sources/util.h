//Header file containing prototypes and globals of misc fucntions

//LCD Macros
#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02

//Utility Assembly Functions---------------------------------------------------

//enableLED--------------------------------------------------------------------
// Initialize LEDs as display on dragon board
// Arguments: 	none
// Output:		none
extern void enableLED(void);

//msDelay--------------------------------------------------------------------
// Creates delay of specified milliseconds
// Arguments:	number of ms to be delayed
// Output: 		none
extern void msDelay(int);

//clockSet--------------------------------------------------------------------
// Sets up PPL to achieve 24Mhz always
// Arguments:	none
// Output: 		none
extern void clockSet(void);

//MOVED TO PIXELS.C

//sendByte--------------------------------------------------------------------
// Sends 
// Arguments: 	int number of ms to be delayed
// Output: 		none
//extern void sendByte(char);


//ALL THESE NEED TO BE MOVED TO A .C FILE, THEIR PROTOTYPES WILL REMAIN HERE (SEE ABOVE)
/*
//Functions for Hex Keypad-----------------------------------------------------

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




//Functions for the LCD --------------------------------------------------------------------------------


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
 
//Functions for Hex Keypad-----------------------------------------------------
*/ 