#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <usart.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define D1_RED      PORTEbits.RE0                           //Lines 13 to 22 define the LEDs D1, D2, and D3
#define D1_GREEN    PORTEbits.RE1
#define D1_BLUE     PORTEbits.RE2 

#define D2_RED      PORTCbits.RC0
#define D2_GREEN    PORTCbits.RC1
#define D2_BLUE     PORTCbits.RC2

#define D3_GREEN   PORTCbits.RC3                                               
#define D3_BLUE    PORTCbits.RC4                                                 

void SET_D1_OFF(void);                                      //Lines 24-31 Initializes the routines for the D1 LED
void SET_D1_RED(void);
void SET_D1_GREEN(void);
void SET_D1_YELLOW(void);
void SET_D1_BLUE(void);
void SET_D1_PURPLE(void);
void SET_D1_CYAN(void);
void SET_D1_WHITE(void);

void SET_D3_OFF(void);                                      //Lines 33-36 Initializes the routines for the D3 LED
void SET_D3_GREEN(void);
void SET_D3_BLUE(void);
void SET_D3_CYAN(void);

void Set_ADCON0 (char AN_pin);                              //Lines 38-46 Initializes the rest of my routines
void Init_ADC(void);
unsigned int Get_Full_ADC(void);
char array[10];
void Display_Lower_Digit(char digit);
void Display_Upper_Digit(char digit);
void RGB_D1(int temp);
void RGB_D2(int temp);
void RGB_D3(int volt);

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
    Init_ADC();                                             //Calls Init_ADC
    init_UART();                                            //Calls Init_UART
    TRISA = 0x0F;                                           //Makes PORTA into an input
    TRISB = 0x00;                                           //Makes PORTB into an output
    TRISC = 0x00;                                           //Makes PORTC into an output
    TRISD = 0x00;                                           //Makes PORTD into an output
    TRISE = 0x00;                                           //Makes PORTE into an output
   
    while(1)
    {
    Set_ADCON0(0);                                          //Select channel AN0
    int num_step = Get_Full_ADC();                          //Retrieves ADC step number at AN0
    float voltage_mv = num_step*4.0;                        //Calculates voltage in mV from the ADC step number
    float temperature_C = (voltage_mv - 1035)/-5.50;        //Calculates temperature (degrees C) using equation 6 in the LMT84 data sheet (pg.11)
    float temperature_F = 1.80*temperature_C + 32.0;        //Conversion of the units of temperature from degrees C to Fahrenheit (F)
    int tempF = (int)temperature_F;                         //Converts data type of temperature (F) from float into type int  
    char U = tempF/10;                                      //Char for the upper digit of the 7 seg
    char L = tempF%10;                                      //Char for the lower digit of the 7 seg
    Display_Upper_Digit(U);                                 //Calls the Display Upper Function
    Display_Lower_Digit(L);                                 //Calls the Display Lower Function
    RGB_D1(tempF);                                          //Calls the RGB_D1 Function for the calculated temperature
    RGB_D2(tempF);                                          //Calls the RGB_D2 Function for the calculated temperature
    
    Set_ADCON0(2);                                          //Select channel AN2
    int num_step1 = Get_Full_ADC();                         //Retrieves ADC step number at AN2
    float voltage1_mv = num_step1*4.0;                      //Calculates voltage in mV from the ADC step number
    int an2_voltage = (int)voltage1_mv;                     //Stores the float voltage into an INT
    RGB_D3(an2_voltage);                                    //Calls the RGB_D3 function for the AN2 Voltage
    printf("Temperature = %d F  |  AN2 Voltage = %d mV \r\n",tempF,an2_voltage);        //Prints to TERATERM
    
    }
}

void Init_ADC(void)
{
    ADCON0 = 0x01;                                          //Select channel AN0, and turn on the ADC subsystem
    ADCON1 = 0x1B;                                          //Select pins AN0 through AN3 as analog signal, (VREF+)-VSS
                                                            //as the reference voltage configuration.
    ADCON2 = 0xA9;                                          //Selects the ADC clock, sets the A/D acquisition time (TAD),
                                                            //and right justify the result.
}

void Set_ADCON0 (char AN_pin)
{
 ADCON0 = AN_pin * 4 + 1;                                   //Select channel, and turn on the ADC subsystem => 00(channel)01
} 

unsigned int Get_Full_ADC(void)
{
    int result;
    ADCON0bits.GO = 1;                                      //Start Conversion
    while(ADCON0bits.DONE == 1);                            //wait for conversion to be completed
    result = (ADRESH*0x100) + ADRESL;                       //combine result of upper byte and lower byte into result
    return result;                                          //return the result
}
//BCD Digit        0     1    2    3   4    5    6    7    8    9
char array[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};       //array for the common anode 7 seg display hex values

void Display_Lower_Digit(char digit)
{
    PORTB = array[digit];                                   //Outputs the array element onto the Lower Digit 7-Segment
}

void Display_Upper_Digit(char digit)
{
    PORTD = array[digit];                                   //Outputs the array element onto the Upper Digit 7-Segment
}

void RGB_D1(int temp)                                       //Uses IF statement to set the color of RGB D1 depending on the temperature
{
    if(temp < 40) SET_D1_OFF();
    if(temp > 39 & temp < 50) SET_D1_RED();
    if(temp > 49 & temp < 60) SET_D1_GREEN();
    if(temp > 59 & temp < 70) SET_D1_YELLOW();
    if(temp > 69 & temp < 72) SET_D1_BLUE();
    if(temp > 71 & temp < 74) SET_D1_CYAN();
    if(temp > 73) SET_D1_WHITE();
}

void RGB_D2(int temp)
{
    int test = temp/10;                                     //Stores the temperature in degree F divided by 10 into the variable test
    if(test > 7) test = 7;                                  //If test > 7, then test = 7.
    PORTC = test;                                           //Outputs the result onto RGB D2
}

void RGB_D3(int volt)                                       //Uses IF statement to set the color of RGB D3 depending on the voltage
{
    if(volt < 2600) SET_D3_OFF();
    if(volt >= 2600 & volt < 3000) SET_D3_GREEN();
    if(volt >= 3000 & volt < 3300) SET_D3_BLUE();
    if(volt >= 3300) SET_D3_CYAN();
}

void SET_D1_OFF()                                           //Method to set D1 to OFF
{
    D1_RED = 0;
    D1_GREEN = 0;
    D1_BLUE = 0;
}

void SET_D1_RED()                                           //Method to set D1 to RED
{
    D1_RED = 1;
    D1_GREEN = 0;
    D1_BLUE = 0;
}

void SET_D1_GREEN()                                         //Method to set D1 to GREEN
{
    D1_RED = 0;
    D1_GREEN = 1;
    D1_BLUE = 0;
}

void SET_D1_YELLOW()                                        //Method to set D1 to YELLOW
{
    D1_RED = 1;
    D1_GREEN = 1;
    D1_BLUE = 0;
}

void SET_D1_BLUE()                                          //Method to set D1 to BLUE
{
    D1_RED = 0;
    D1_GREEN = 0;
    D1_BLUE = 1;
}

void SET_D1_PURPLE()                                        //Method to set D1 to PURPLE
{
    D1_RED = 1;
    D1_GREEN = 0;
    D1_BLUE = 1;
}

void SET_D1_CYAN()                                          //Method to set D1 to CYAN
{
    D1_RED = 0;
    D1_GREEN = 1;
    D1_BLUE = 1;
}

void SET_D1_WHITE()                                         //Method to set D1 to WHITE
{
    D1_RED = 1;
    D1_GREEN = 1;
    D1_BLUE = 1;
}

void SET_D3_OFF()                                           //Method to set D3 to OFF
{
    D3_GREEN = 0;
    D3_BLUE = 0;
}


void SET_D3_GREEN()                                         //Method to set D3 to GREEN
{
    D3_GREEN = 1;
    D3_BLUE = 0;
}

void SET_D3_BLUE()                                          //Method to set D3 to BLUE
{
    D3_GREEN = 0;
    D3_BLUE = 1;
}

void SET_D3_CYAN()                                          //Method to set D3 to CYAN
{
    D3_GREEN = 1;
    D3_BLUE = 1;
}
