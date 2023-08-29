#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

// Function to generate 1 second delay
void Delay_One_Sec()
{
  for(int i=0; i < 17000; i++);
}

void main() 
{
  TRISC = 0x00;                     // Set PORT C as output
  while (1)
  {
    for (int i = 0; i <= 7; i++)    // Loop 8 times to generate 8 different colors
    {
      PORTC = i;                    // Set the output of PORTC to the value of the counter
      Delay_One_Sec();              // Generate 1 second delay
    }
  }
}
