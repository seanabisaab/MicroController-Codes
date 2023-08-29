#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <usart.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTC

#define DP PORTDbits.RD7

#define D1_BLUE PORTEbits.RE0
#define D1_RED PORTEbits.RE1

#define D2_RED PORTCbits.RC3
#define D2_GREEN PORTCbits.RC4
#define D2_BLUE PORTCbits.RC5

void RGB_D2(int res);

void Set_ADCON0 (char AN_pin)
{
 ADCON0 = AN_pin * 4 + 1;                                                   //Select channel, and turn on the ADC subsystem => 00(channel)01
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

void Wait_1_Sec()
{
    for(int j = 0;j < 0xFFFF;j++);                                                  //Waits one second before proceeding to the next line
}

void SET_D2_OFF()                                                                   //Sets D2 to OFF
{
    D2_RED = 0;
    D2_GREEN = 0;
    D2_BLUE = 0;
}

void SET_D2_RED()                                                                   //Sets D2 to RED
{
    D2_RED = 1;
    D2_GREEN = 0;
    D2_BLUE = 0;
}

void SET_D2_GREEN()                                                                 //Sets D2 to GREEN
{
    D2_RED = 0;
    D2_GREEN = 1;
    D2_BLUE = 0;
}

void SET_D2_YELLOW()                                                                //Sets D2 to YELLOW
{
    D2_RED = 1;
    D2_GREEN = 1;
    D2_BLUE = 0;
}

void SET_D2_BLUE()                                                                  //Sets D2 to BLUE
{
    D2_RED = 0;
    D2_GREEN = 0;
    D2_BLUE = 1;
}

void SET_D2_PURPLE()                                                                //Sets D2 to PURPLE
{
    D2_RED = 1;
    D2_GREEN = 0;
    D2_BLUE = 1;
}

void SET_D2_CYAN()                                                                  //Sets D2 to CYAN
{
    D2_RED = 0;
    D2_GREEN = 1;
    D2_BLUE = 1;
}

void SET_D2_WHITE()                                                                 //Sets D2 to WHITE
{
    D2_RED = 1;
    D2_GREEN = 1;
    D2_BLUE = 1;
}

void SET_D1_OFF()                                                                   //Sets D1 to OFF
{
    D1_RED = 0;
    D1_BLUE = 0;
}

void SET_D1_RED()                                                                   //Sets D1 to RED
{
    D1_RED = 0;
    D1_BLUE = 1;
}

void SET_D1_PURPLE()                                                                //Sets D1 to PURPLE
{
    D1_RED = 1;
    D1_BLUE = 1;
}

void RGB_D2(int res)                                                                //Function to control the color of D2 LED. 
{                                                                                   //Uses if statements with the measured resistor value.
    if(res < 10) SET_D2_OFF();
    if(res > 10 & res < 19) SET_D2_RED();
    if(res > 20 & res < 29) SET_D2_GREEN();
    if(res > 30 & res < 39) SET_D2_YELLOW();
    if(res > 40 & res < 49) SET_D2_BLUE();
    if(res > 50 & res < 59) SET_D2_PURPLE();
    if(res > 60 & res < 69) SET_D2_CYAN();
    if(res > 70) SET_D2_WHITE();
}

void Activate_Buzzer()                                                              //Function to activate the Buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
} 

void Deactivate_Buzzer()                                                            //Function to deactivate the Buzzer
{
    CCP2CON = 0x0;
    PORTCbits.RC1 = 0;
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
        Set_ADCON0(2);                                                                     //Set to channel 2
        int num_step = Get_Full_ADC();                                                     //Retrieves ADC step number at AN2
        float voltage_mv = num_step*4.0;                                                   //Calculates voltage in mV from the ADC step number
        float AN_voltage = voltage_mv/1000.0;                                              //Calculates voltage in V
        float RL_resistance = (AN_voltage*1)/(4.096-AN_voltage);                           //Calculates resistance from voltage
        printf("RL_Resistor = %f k ohms\r\n",RL_resistance);                               //Prints to Teraterm
       
        if(RL_resistance < 10.0)                                                           //Prints to 7 seg for numbers greater than 10k ohm
        {
            char U = (int)RL_resistance;
            char L = (int)((RL_resistance-U)*10);
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);
            DP = 0;
        }
        else                                                                               //Prints to 7 seg for numbers less than 10k ohm
        {
            int RL_Int = (int)RL_resistance;
            char U = RL_Int/10;
            char L = RL_Int%10;
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);
            DP = 1;
        }
        
        if(RL_resistance < 0.070)                                                //Lines 217 to 231 control the color of LED D1 and the Buzzer            
        {
            SET_D1_PURPLE();
            Activate_Buzzer();
        }
        if(RL_resistance > 0.1)
        {
            SET_D1_OFF();
            Deactivate_Buzzer();
        }
        if(RL_resistance > 0.069 & RL_resistance < 0.101) 
        {
            SET_D1_RED();
            Deactivate_Buzzer();
        }
        
        RGB_D2(RL_resistance);                                                             //Calls function for the D2 LED
        Wait_1_Sec();                                                                      //Call delay function
    }
}
