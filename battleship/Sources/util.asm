
      XDEF msDelay, LED_enable, sendByte
      

  Include 'mc9s12dg256.inc'


BIT1:   EQU %00000010


;******************************************
;Enable the 8 LEDs
; Input/Ouptut Parameters:
;   None
; Affected Registers
;   None
;******************************************
LED_enable:
          BSET DDRJ, BIT1   ; config PJ1 as output pin
          BCLR PTJ,  BIT1   ; drive PJ1 low
          
          MOVB #$FF, DDRB   ; config PB7-PB0 as output pins
          CLR PORTB         ; starts off          
          RTS

;******************************************
;Sends serial 1 or 0 across PB0
; Input Register:
;   1 or 0 is passed in on B as boolean
; Output Register:
;   none
;  
;************************************************
sendByte:                 ;B contains the char to be sent out serially
              LDAA #08    ;Count of bits
      next:   CMPB #00    ;Bring B back in focuse
              BMI send1     ;Check if MSB is 0 or 1
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
              NOP             ;High pulse of 706ns
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
    

           
;******************************************
; delay by # of ms pushed onto stack
; Input Parameter
;   unsigned int of ms to delay
; Output Parameter:
;   None
; Affected Registers
;   None
;************************************************

msDelay:                                    
	          PSHX                                ;    2  clock cycles
     Outer: 
            LDX  #7995                           ;    2     
     Inner: 
            DBNE X,  Inner                  ;     3/3    ( 3 cycles both ways)
	          
	          DBNE D, Outer                  ;     3/3
            PULX                               ;     3 
 	          RTS                                 ;    5 

