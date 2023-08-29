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
char in;                    // Use variable ?in? as char
    TRISA = 0x0F;           // we just made port A input 
    TRISB = 0x00;           // we just made port B output
    TRISC = 0x00;           // we made port C an output 
    ADCON1 = 0x0F;          // This makes the ports A&B All digital 

 while (1)
{
in = PORTA;                 // read data from PORTA and save it into ?in?
 in = in & 0x0F;            // Mask out the unused upper four bits
 PORTC = in;                // Output the data to PORTC
}
 } 