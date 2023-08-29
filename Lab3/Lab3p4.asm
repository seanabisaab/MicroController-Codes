#include <P18F4620.inc> ; Include the PIC18F4620 header file

config OSC = INTIO67 
config WDT = OFF 
config LVP = OFF 
config BOREN = OFF 

Counter_L equ 0x20 
Counter_H equ 0x21 
Color_Value equ 0x28
Loop_Count equ 0x29 

Delay1 equ 0xFF 
Delay2 equ 0xFF 

ORG 0x0000			; Start of program

START:
 ORG 0x0000 
START: 
 MOVLW 0x0F			; Load W register with value 0x0F
 MOVWF ADCON1		; Move value from W register to ADCON1 
 MOVLW 0x00			; Load W register with value 0x00
 MOVWF TRISC		; Move value from W register to TRISC to configure PORTC pins as output

WHILE_LOOP:					; Label for the start of the while loop
 MOVLW 0x00					; Load W register with value 0x00
 MOVWF Color_Value			; Move value from W register to Color_Value memory address to set initial color
 MOVLW 0x08					; Load W register with value 0x08
 MOVWF Loop_Count			; Move value from W register to Loop_Count memory address to set loop count

FOR_LOOP:					; Label for the start of the for loop
 MOVF Color_Value,W			; Move value from Color_Value memory address to W 
 MOVWF PORTC				; Move value from W register to PORTC to output color
 CALL DELAY_ONE_SEC			; Call the delay subroutine to wait for one second
 INCF Color_Value,F			; Increment the value in Color_Value memory address
 DECF Loop_Count,F			; Decrement the value in Loop_Count memory address
 BNZ FOR_LOOP				; If Loop_Count is not zero, go back to start of for loop

 GOTO WHILE_LOOP			; If Loop_Count is zero, go back to start of while loop

 DELAY_ONE_SEC:
MOVLW Delay1				; Load W with value of Delay1
MOVWF Counter_H				; Move value from W register to Counter_H
LOOP_OUTER: 
NOP 
MOVLW Delay2				; Load W with value of Delay2
MOVWF Counter_L				; Move value from W register to Counter_L
LOOP_INNER:
NOP 
DECF Counter_L,F			; Decrement the value in Counter_L 
BNZ LOOP_INNER				; If Counter_L is not zero, go back to start of inner loop
DECF Counter_H,F			; Decrement the value in Counter_H 
BNZ LOOP_OUTER				; If Counter_H is not zero, go back to start of outer loop
RETURN

END