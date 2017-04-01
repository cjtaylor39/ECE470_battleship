#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"
#include "pixels.h"
#include "spi.h"

#define DUMMY 0x05


void transition(void){
  int i = 0;
  int j = 0;
  int off = 0;
  for (j = 0; j < 64; j++) {
    for (i = 0; i < off; i++) {
      sendPixel(0,0,0);
      j++;
    }
    sendPixel(0,0,10);
    off++;
  }
  msDelay(50);
}

//Belongs in util.h?
typedef enum state_type {
  ATTACK,
  DEFEND  
} state_type;


void main(void){
  
  //Matricies 
  char defense_large[10][10] = {0};
  char defense[8][8] = {0};
  char attack[8][8] = {0};
  
  
  char option;
 
  char save1 = 0;
  char select = 0;
  char send_cord = 0;
  unsigned char X=3;
  unsigned char Y=3;
  char hitmiss = 0;
  char received=0;
  char xdef = 0; char Ydef = 0;
  state_type state = ATTACK; 
  unsigned char dataIn = 0;
  unsigned char dataOut = 0;
  
  unsigned char test_var = 0;
  
  enableLEDs();
  DDRH = 0x00;
  
       
        lcdInit();
  
  

  while(state == ATTACK){
      
      dispLCD(1);
     
     
      //display attack board
  
     
      save1 = attack[X][Y];  // save what was in the location we will replace with 5
      attack [X][Y] = 5;    // assign the value of a pointer (different collor) to the matrix
      displayBoard(attack);    
      
      while(1){
           
         select =hexKeypad();
         
         if(select == 2){
           attack[X][Y]=save1;
             X-= 1;
             X = X% 8;
           save1=attack[X][Y];
           attack[X][Y]=5;
           displayBoard(attack);
         } else if(select == 4){
             attack[X][Y]=save1;
             Y -= 1;
             Y = Y% 8;
             save1=attack[X][Y];
             attack[X][Y]=5;
            displayBoard(attack);
         } else if(select == 6){
             attack[X][Y]=save1;
             Y+= 1;
             Y = Y% 8;
             save1=attack[X][Y];
             attack[X][Y]=5;
           displayBoard(attack);
         } else if(select == 8){
             attack[X][Y]=save1;
             X += 1;
             X = X% 8;
             save1=attack[X][Y];
             attack[X][Y]=5;
            displayBoard(attack);
         } else if(select == 5){
            if(save1!=0){
            select = 0;
            } 
            else {
              break;
            }
         }
       }
      
      
       send_cord = X;
       send_cord<<4;
       send_cord+=Y;
       
       
       attack[X][Y] = 3;
       displayBoard(attack);
       dispLCD(2);

  
}

}



