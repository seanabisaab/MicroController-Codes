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
char InA;
char Result;
ADCON1 = 0x0F;
 TRISA = 0xFF;              // make sure PORT A is input
 TRISB = 0xFF;              // make sure PORT B is input
 TRISC = 0x00;              // make sure PORT C is output
 while (1)
{
 InA = PORTA;               // Read data from PORTA and save it into InA
 InA = InA & 0x0F;          // Mask out the unused upper four bits
 Result = ~InA;             // Set Result to the inverse of InA
 PORTC = Result;            // Output the Data to PortC
 }
} 



