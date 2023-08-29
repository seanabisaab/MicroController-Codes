#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

void main()
{
char in;                   // Use variable ?in? as char
    TRISA = 0x0F;          // Set PORTA as input (1 = input, 0 = output)
    TRISB = 0x00;          // Set PORTB as output (1 = input, 0 = output)
    ADCON1 = 0x0F;         // Set all PORTA and PORTB pins as digital (not analog)


 while (1)
    {
    in = PORTA;            // read data from PORTA and save it into in
    in = in & 0x0F;        // Mask out the unused upper four bits while preserving the lower 4-bits
    PORTB = in;            // Output the data to PORTB
    }   
 }
