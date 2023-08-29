#include<P18F4620.inc>
    config  OSC = INTIO67
    config  WDT = OFF
    config  LVP = OFF
    config  BOREN = OFF
   
    ORG    0x0000
 
START:
    InA    equ  0x25    
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
    COMF	InA, 0		    ;Take 1's comp and store in W
    ANDLW	0x0F		    ;And W with the mask of 0x0F 
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    BZ		SET_E		    ;Jump to SET_E label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    GOTO	MAIN_LOOP	    ;Go back to the main loop
    
    SET_E:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    GOTO	MAIN_LOOP	    ;Go back to the main loop
   
    END


