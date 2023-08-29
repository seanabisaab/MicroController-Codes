#include<P18F4620.inc>
    config  OSC = INTIO67
    config  WDT = OFF
    config  LVP = OFF
    config  BOREN = OFF
    
    ORG	    0x0000
 
;CODE STARTS FROM THE NEXT LINE
START:
    
    InA	    EQU	    0x25		    
    Result  EQU	    0x27

    MOVLW	0x0F		    ;Load W with 0x0F
    MOVWF	ADCON1		    ;Stores W into ADCON1 and makes it digital mode    
    MOVLW	0xFF		    ;Load W with 0xFF
    MOVWF	TRISA		    ;Stores W into PORT A which makes it an input   
    MOVLW	0xFF		    ;Load W with 0xFF
    MOVWF	TRISB		    ;Stores W into PORT B which makes it an input   
    MOVLW	0x00		    ;Load W with 0x00
    MOVWF	TRISC		    ;Stores W into PORT C which makes it an output
    
MAIN_LOOP: 
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    COMF	InA, 0		    ;Take 1's comp and store in W
    ANDLW	0x0F		    ;And W with the mask of 0x0F 
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    GOTO	MAIN_LOOP	    ;Goes back to MAIN_LOOP to loop forever
   
    END