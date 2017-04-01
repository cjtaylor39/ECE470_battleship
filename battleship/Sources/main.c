#include <hidef.h>      /* common defines and macros */
#include <stdlib.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "util.h"
#include "pixels.h"

#include "sci.h"

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
  DEFEND,
  WIN,
  LOSE  
} state_type;

//Global defense, will move later when random generated
char defense[8][8] = {0,0,0,0,0,0,0,0,
                     0,1,1,1,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,9,1,1,1,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,1,0,
                     0,0,0,0,0,0,1,0};

void main(void){
  
  //Matricies
  //char defense_large[10][10] = {0};
  //char defense[8][8] = {0};
  char attack[8][8] = {0};
  
  //Sending coordinates stuff
  int hit_count = 0;
  char save1 = 0;      //Coordinate contents to be remembered
  char select = 0;     //HexKeypad Coordinate
  char send_coord = 0; //Byte to be sent
  unsigned char x=3;   //x component of moving pixel
  unsigned char y=3;   //y component of moving pixel
  char hit_miss = 0;    //byte returned from other board signifying hit/miss/gameover
  
  //char* def_ptr_small;
  //char* def_ptr_large;
  
  state_type state = ATTACK; 
  unsigned char dataIn = 0;
  unsigned char dataOut = 0;
 
  //Enable functionalities
  enableLEDs();
  DDRH = 0x00;     
  lcdInit();
  configureSCI();
  
  //Generate Random Defense Matrix
  //def_ptr_small = &defense[0][0];
  //def_ptr_large = &defense_large[0][0];
  
  //Rand() is broken within this project
  //matrixBuild(def_ptr_large, def_ptr_small);
  
  displayBoard(defense);
  msDelay(5000);
  //This is your board on LCD
  
  while(state == ATTACK){
  
      //LCD says it is your turn
      dispLCD(1);
     
      //display attack board
      save1 = attack[x][y];  // save what was in the location we will replace with 5
      attack [x][y] = 5;    // assign the value of a pointer (different collor) to the matrix
      displayBoard(attack);    
      
      //Selecting coordinate to attack
      while(1){
           
         select = hexKeypad();
         
         if(select == 2){
           attack[x][y]=save1;
             x-= 1;
             x = x% 8;
           save1=attack[x][y];
           attack[x][y]=5;
           displayBoard(attack);
         } else if(select == 4){
             attack[x][y]=save1;
             y -= 1;
             y = y% 8;
             save1=attack[x][y];
             attack[x][y]=5;
            displayBoard(attack);
         } else if(select == 6){
             attack[x][y]=save1;
             y+= 1;
             y = y% 8;
             save1=attack[x][y];
             attack[x][y]=5;
           displayBoard(attack);
         } else if(select == 8){
             attack[x][y]=save1;
             x += 1;
             x = x% 8;
             save1=attack[x][y];
             attack[x][y]=5;
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
      
      
       send_coord = x;
       send_coord <<= 4;
       send_coord += y;
       
       sendByteSCI(send_coord);
       hit_miss = receiveByteSCI();
       
       //LCD displays hit/miss/gg
       if (hit_miss == HITMSG) {
          attack[x][y] = HIT;
          dispLCD(HIT);
          hit_count++;   
       }
       else if(hit_miss == MISSMSG) {
          attack[x][y] = MISS;
          dispLCD(MISS);
       }
       else {
        //Display Transmission error on LCD
        PORTB |= 0xF0;
        //while(1); 
       }
       
       displayBoard(attack);
       msDelay(3000);
       //Your turn to defend
       
       if (hit_count != 12) { 
          state = DEFEND; 
       }
       else { 
          state = WIN; 
       }
  }
  

}



