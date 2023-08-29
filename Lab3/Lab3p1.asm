; THIS FIRST ASSEMBLY LANGUAGE PROGRAM WILL FLASH LEDS
; CONNECTED TO THE PINS 0 THROUGH 3 OF PORT B
#include<P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
; Constant declarations
Delay1 equ 0xFF
Delay2 equ 0XFF
Counter_L equ 0x20
Counter_H equ 0x21
ORG 0x0000
 ; CODE STARTS FROM THE NEXT LINE
START:
 MOVLW 0x0F					; Set W to 0x0F
 MOVWF ADCON1				; Move the contents of W to the ADCON1 register
 MOVLW 0x00					; Set the W to 0x00
 MOVWF TRISB				; Move the contents of W to TRISB to set all pins of PORTB as outputs
MAIN_LOOP:
MOVLW 0x05					; Set W to 0x05
MOVWF PORTB					; Move the contents of W to PORTB to turn on LEDs connected to pins 0-3 of PORTB
 CALL DELAY_ONE_SEC			; Call the delay function
MOVLW 0x0A					; Set W to 0x0A
MOVWF PORTB					; Move the contents of W to PORTB to turn off LEDs connected to pins 0-3 of PORTB
 CALL DELAY_ONE_SEC			; Call the delay function
 GOTO MAIN_LOOP				; Go to MAIN_LOOP to repeat sequence
DELAY_ONE_SEC:
MOVLW Delay1				; Set W to Delay1 constant
MOVWF Counter_H				; Move the contents of W to Counter_H
LOOP_OUTER:
NOP ;
MOVLW Delay2				; Set W to Delay2 constant
MOVWF Counter_L				; Move the contents of W to Counter_L
LOOP_INNER:
NOP 
DECF Counter_L,F			; Decrement Counter_L by 1
BNZ LOOP_INNER				; Branch to LOOP_INNER label if the result of the decrement is not zero
DECF Counter_H,F			; Decrement Counter_H by 1
BNZ LOOP_OUTER				; Branch to LOOP_OUTER label if the result of the decrement is not zero
RETURN
 END


