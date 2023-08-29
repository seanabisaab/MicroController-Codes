#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p18f4620.h>
#include "utils.h"



void Do_Beep()
{
    Activate_Buzzer();                                                              // Activates Buzzer   
    Wait_One_Sec();                                                                   // Waits one second 
    Deactivate_Buzzer();                                                            // Deactivates Buzzer
}

void Activate_Buzzer()
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;                      
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
	PORTCbits.RC1 = 0;
}


void Wait_One_Sec()
{
    for (int k=0;k<30000;k++);
}

char bcd_2_dec (char bcd)
{
    int dec;
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}





