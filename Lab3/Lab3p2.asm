; THIS SECOND ASSEMBLY LANGUAGE PROGRAM WILL READ THE VALUES OF
; ALL THE BITS 0-3 OF PORT A AND OUTPUT THEM
; TO THE PINS 0 THROUGH 3 OF PORT B
#include <P18F4620.inc>
 config OSC = INTIO67
 config WDT = OFF
 config LVP = OFF
 config BOREN = OFF
 ORG 0x0000
START:
 MOVLW 0x0F					; Set W to 0x0F
 MOVWF ADCON1				; Move the contents of W to the ADCON1 register
 MOVLW 0xFF					; Set W to 0xFF
 MOVWF TRISA				; Move the contents of W to TRISA to configure all pins of PORTA as inputs
 MOVLW 0x00					; Set W to 0x00
 MOVWF TRISB				; Move the contents of W to TRISB to configure all pins of PORTB as outputs
MAIN_LOOP: 
 MOVF PORTA, W				; Move the contents of PORTA to W
 ANDLW 0x0F					; Perform a bitwise AND with 0x0F to isolate the lower four bits
 MOVWF PORTB				; Move the contents of W to PORTB to output the lower four bits of PORTA
 GOTO MAIN_LOOP				; Repeat the Sequence
 END 



