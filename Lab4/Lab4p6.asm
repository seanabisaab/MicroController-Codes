#include<P18F4620.inc>
    config  OSC = INTIO67
    config  WDT = OFF
    config  LVP = OFF
    config  BOREN = OFF
   
    ORG    0x0000
 
START:
    InA    equ  0x25
    InB	    equ 0x29
    Result  equ  0x27

    MOVLW   0x0F	;Load W with 0x0F
    MOVWF   ADCON1	;Stores W into ADCON1 and makes it digital mode
    MOVLW   0xFF	;Load W with 0xFF
    MOVWF   TRISA	;Stores W into PORT A which makes it an input
    MOVLW   0xFF	;Load W with 0xFF
    MOVWF   TRISB	;Stores W into PORT B which makes it an input
    MOVLW   0x00	;Load W with 0x00
    MOVWF   TRISC	;Stores W into PORT C which makes it an output
    MOVLW   0x07	;Load W with 0x07
    MOVWF   TRISD	;Stores W into PORT D which makes it a mix of inputs and outputs
    MOVLW   0x00	;Load W with 0x00
    MOVWF   TRISE	;Stores W into PORT E which makes it an output
   
MAIN_LOOP:
;start of While LOOP
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    MOVLW	0x09		    ;Load W with 0x09
    CPFSGT	InA, 0		    ;If InA is greater than W goto ADD6, else go to DISPLAY
    GOTO	DISPLAY
    GOTO	ADD6
    
ADD6:
    MOVLW	0x06		    ;Load W with 0x06
    ADDWF	InA,0		    ;Add 0x06 to InA
    MOVWF	Result		    ;Move W to result
    MOVFF	Result,PORTC	;Move RESULT to portC
    GOTO	MAIN_LOOP
    
DISPLAY:
    MOVF	InA,0		    ;Move W to InA
    MOVFF	InA,PORTC	    ;Move RESULT to portC 
    GOTO	MAIN_LOOP

END




