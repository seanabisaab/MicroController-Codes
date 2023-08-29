#include <P18F4620.inc> ; Include the PIC18F4620 header file

config OSC = INTIO67        ; Configure oscillator to use internal clock
config WDT = OFF            ; Disable Watchdog Timer
config LVP = OFF            ; Disable Low Voltage Programming
config BOREN = OFF          ; Disable Brown-out Reset

Delay1 equ 0xFF             ; Define a delay constant
Delay2 equ 0xFF             ; Define another delay constant
Counter_L equ 0x20
Counter_H equ 0x21
Color_Value equ 0x28
Loop_Count equ 0x29
 
D1_Start_Value equ 0x50
D2_Start_Value equ 0x60

ORG 0x0000

START: 
    ORG 0x0000
    MOVLW 0x0F          ; Set W to 0x0F
    MOVWF ADCON1        ; Move value from W register to ADCON1 
    MOVLW 0x00          ; Set W to 0x00
    MOVWF TRISC         ; Set PORTC as output
    MOVWF TRISD         ; Set PORTD as output
    
                        ; Array for D1 LED
                        ; Displays LEDs with their designated colors from 0 to 7
    MOVLW 0x0
    MOVWF D1_Start_Value
    MOVLW 0x1
    MOVWF D1_Start_Value+1
    MOVLW 0x2
    MOVWF D1_Start_Value+2
    MOVLW 0x3
    MOVWF D1_Start_Value+3
    MOVLW 0x4
    MOVWF D1_Start_Value+4
    MOVLW 0x5
    MOVWF D1_Start_Value+5
    MOVLW 0x6
    MOVWF D1_Start_Value+6
    MOVLW 0x7
    MOVWF D1_Start_Value+7
    
                        ; Array for D2 LED
                        ; Displays LEDs as designated in the instructions
    MOVLW 0x6
    MOVWF D2_Start_Value
    MOVLW 0x2
    MOVWF D2_Start_Value+1
    MOVLW 0x7
    MOVWF D2_Start_Value+2
    MOVLW 0x3
    MOVWF D2_Start_Value+3
    MOVLW 0x4
    MOVWF D2_Start_Value+4
    MOVLW 0x5
    MOVWF D2_Start_Value+5
    MOVLW 0x0
    MOVWF D2_Start_Value+6
    MOVLW 0x1
    MOVWF D2_Start_Value+7

    
WHILE_LOOP: 
    LFSR 0, D1_Start_Value              ; Load FSR0 with the address of the start of the D1 array
    LFSR 1, D2_Start_Value              ; Load FSR1 with the address of the start of the D2 array

    MOVLW 0x08                          ; Set W to 0x08
    MOVWF Loop_Count                    ; Move the value from W into Loop_Count

FOR_LOOP:                          
    MOVF INDF0,W                        ; Move the value of the data pointed to by FSR0 into W
    MOVWF PORTC                         ; Move the value in W to PORTC
    MOVF INDF1,W                        ; Move the value of the data pointed to by FSR1 into W
    MOVWF PORTD                         ; Move the value in W to PORTD
    CALL DELAY_ONE_SEC                  ; Call the delay function
    INCF FSR0L                          ; Increment the low byte of FSR0 to point to the next data in the D1 array
    INCF FSR1L                          ; Increment the low byte of FSR1 to point to the next data in the D2 array
    
    DECF Loop_Count,F                   ; Decrement the Loop_Count value and store the result in Loop_Count
    BNZ FOR_LOOP                        ; If Loop_Count is not zero, go back to the beginning of the for loop

    GOTO WHILE_LOOP                     ; Go back to the beginning of the while loop

DELAY_ONE_SEC:                          ; Subroutine for delay
MOVLW Delay1                            ; Load W register with value of Delay1
MOVWF Counter_H                         ; Move value from W register to Counter_H memory address
LOOP_OUTER:                             ; Label for the outer loop
NOP                                     ; No operation
MOVLW Delay2                            ; Load W register with value of Delay2
MOVWF Counter_L                         ; Move value from W register to Counter_L memory address
LOOP_INNER:                             ; Label for the inner loop
NOP                                     ; No operation
DECF Counter_L,F                        ; Decrement the value in Counter_L memory address
BNZ LOOP_INNER                          ; If Counter_L is not zero, go back to start of inner loop
DECF Counter_H,F                        ; Decrement the value in Counter_H memory address
BNZ LOOP_OUTER                          ; If Counter_H is not zero, go back to start of outer loop
RETURN 

END
