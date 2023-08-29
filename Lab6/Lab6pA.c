#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <usart.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define DP PORTDbits.RD7

void Set_ADCON0 (char AN_pin)
{
 ADCON0 = AN_pin * 4 + 1;                                                     //Select channel, and turn on the ADC subsystem => 00(channel)01
} 

unsigned int Get_Full_ADC(void)
{
    int result;
    ADCON0bits.GO = 1;                                                              //Start Conversion
    while(ADCON0bits.DONE == 1);                                                    //wait for conversion to be completed
    result = (ADRESH*0x100) + ADRESL;                                               //combine result of upper byte and
                                                                                    //lower byte into result
    return result;                                                                  //return the result
}

//BCD Digit        0     1    2    3   4    5    6    7    8    9
char array[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};               //array for the common anode 7 seg display hex values


void Init_ADC(void)
{
    ADCON1 = 0x19;                                                                  //Select pins AN0 through AN3 as analog signal, (VREF+)-VSS
                                                                                    //as the reference voltage configuration.
    ADCON2 = 0xA9;                                                                  //Selects the ADC clock, sets the A/D acquisition time (TAD),
                                                                                    //and right justify the result.
}

void Display_Lower_Digit(char digit)
{
    PORTB = array[digit];                                                           //Outputs the array element onto the Lower Digit 7-Segment LED
}

void Display_Upper_Digit(char digit)
{
    PORTD = array[digit];                                                           //Outputs the array element onto the Upper Digit 7-Segment LED
    
}

void Wait_1_Sec(void)
{
    for(int j = 0;j < 0xFFFF;j++);                                                  //Waits one second before proceeding to the next line
}

void init_UART(void)                                                               
{
 OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
USART_BRGH_HIGH, 25);
 OSCCON = 0x60;
}

void putch (char c)
{      
 while (!TRMT);
 TXREG = c;
}

void main()
{
    Init_ADC();
    init_UART();
   
    TRISB = 0x00;                                                                          //Set to output
    TRISC = 0x00;                                                                          //Set to output
    TRISD = 0x00;                                                                          //Set to output
    TRISE = 0x00;                                                                          //Set to output
   
    while(1)
    {
        Set_ADCON0(0);                                                                     //Set to channel 0
        int num_step = Get_Full_ADC();                                                     //Retrieves ADC step number at AN0
        float voltage_mv = num_step*4.0;                                                   //Calculates voltage in mV from the ADC step number
        float AN0_voltage = voltage_mv/1000.0;                                             //Calculates voltage in V
        char U = (int)AN0_voltage;                                                         //Value for Upper 7 seg digit
        char L = (AN0_voltage - U)*10;                                                     //Value for Lower 7 seg digit
        Display_Upper_Digit(U);                                                            //Displays upper digit
        DP = 0;                                                                            //Turns decimal point on
        Display_Lower_Digit(L);                                                            //Displays lower digit
       
        printf("AN0_voltage = %f V\r\n",AN0_voltage);                                      //Prints to Teraterm
        Wait_1_Sec();                                                                      //Call delay function
    }
}
