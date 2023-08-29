#include<P18F4620.inc>
    config OSC = INTIO67
    config WDT = OFF
    config LVP = OFF
    config BOREN = OFF
   
;Constant declarations
Delay1 equ 0xFF
Delay2 equ 0xFF
ORG 0x0000
 

START:
   
    MOVLW 0x0F              ; Set W to 0x0F
    MOVWF ADCON1            ; Move the contents of W to the ADCON1 register
    MOVLW 0x0F              ; Load W with 0x0F
    MOVWF TRISA             ; Move the contents of W to TRISA to configure all pins of PORTA as inputs
   
    MOVLW 0x00              ; Set W to 0x00
    MOVWF TRISC             ; Move the contents of W to TRISB to configure all pins of PORTB as outputs
   
MAIN_LOOP:
   
    MOVF PORTA,W            ; Move the contents of PORTA to W
    ANDLW 0x0F              ; Perform a bitwise AND with 0x0F to isolate the lower four bits
    MOVWF PORTC             ; Move the contents of W to PORTC
   
    GOTO MAIN_LOOP          ; Repeat the Sequence
   
    END