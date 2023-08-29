#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <usart.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF


#define SEC_LED PORTEbits.RE2
#define MODE_LED PORTDbits.RD7

#define OFF 0
#define RED 1
#define GREEN 2
#define YELLOW 3

//D4
#define NS_RED PORTEbits.RE0                                                        //Define all bits and ports
#define NS_GREEN PORTEbits.RE1
//D3
#define NSLT_RED PORTCbits.RC4
#define NSLT_GREEN PORTCbits.RC5
//D2
#define EW_RED PORTCbits.RC2
#define EW_GREEN PORTCbits.RC3
//D1
#define EWLT_RED PORTAbits.RA5
#define EWLT_GREEN PORTCbits.RC0

#define NSPED_SW PORTAbits.RA4
#define NSLT_SW PORTAbits.RA2
#define EWPED_SW PORTAbits.RA3
#define EWLT_SW PORTAbits.RA1

void Wait_Half_Second()
{
    T0CON = 0x02;                                                                   // Timer 0, 16-bit mode, prescaler 1:8
    TMR0L = 0xDB;                                                                   // set the lower byte of TMR
    TMR0H = 0x0B;                                                                   // set the upper byte of TMR
    INTCONbits.TMR0IF = 0;                                                          // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                                                           // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);                                                 // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                                                           // turn off the Timer 0
} 

void Wait_One_Second()                                              
{
    SEC_LED = 1;                                                                    // First, turn on the SEC LED
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                                                    // then turn off the SEC LED
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
}

void Wait_N_Seconds (char seconds)                                                  // Waits amount of time based on input
{
    char I;
    for (I = 0; I< seconds; I++)
    {
        Wait_One_Second();
    }
} 

void SET_EW(char color)
{
 switch (color)
 {
 case OFF: EW_RED =0;EW_GREEN=0;break;                                              // Turns off the EW LED
 case RED: EW_RED =1;EW_GREEN=0;break;                                              // Sets EW LED RED
 case GREEN: EW_RED =0;EW_GREEN=1;break;                                            // sets EW LED GREEN
 case YELLOW: EW_RED =1;EW_GREEN=1;break;                                           // sets EW LED YELLOW
 }
}

void SET_EWLT(char color)
{
 switch (color)
 {
 case OFF: EWLT_RED =0;EWLT_GREEN=0;break;                                          // Turns off the EWLT LED
 case RED: EWLT_RED =1;EWLT_GREEN=0;break;                                          // Sets EWLT LED RED
 case GREEN: EWLT_RED =0;EWLT_GREEN=1;break;                                        // sets EWLT LED GREEN
 case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break;                                       // sets EWLT LED YELLOW
 }
}

void SET_NS(char color)
{
 switch (color)
 {
 case OFF: NS_RED =0;NS_GREEN=0;break;                                              // Turns off the NS LED
 case RED: NS_RED =1;NS_GREEN=0;break;                                              // Sets NS LED RED
 case GREEN: NS_RED =0;NS_GREEN=1;break;                                            // sets NS LED GREEN
 case YELLOW: NS_RED =1;NS_GREEN=1;break;                                           // sets NS LED YELLOW
 }
}

void SET_NSLT(char color)
{
 switch (color)
 {
 case OFF: NSLT_RED =0;NSLT_GREEN=0;break;                                          // Turns off the NSLT LED
 case RED: NSLT_RED =1;NSLT_GREEN=0;break;                                          // Sets NSLT LED RED
 case GREEN: NSLT_RED =0;NSLT_GREEN=1;break;                                        // sets NSLT LED GREEN
 case YELLOW: NSLT_RED =1;NSLT_GREEN=1;break;                                       // sets NSLT LED YELLOW
 }
}

void Set_ADCON0 (char AN_pin)
{
 ADCON0 = AN_pin * 4 + 1;                                                           //Select channel, and turn on the ADC subsystem => 00(channel)01
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

void Init_ADC(void)
{
    ADCON0 = 0x0E;   
    ADCON1 = 0x0F;                                                                  //Select pins AN0 through AN5 as analog signal, (VREF+)-VSS
                                                                                    //as the reference voltage configuration.
    ADCON2 = 0xA9;                                                                  //Selects the ADC clock, sets the A/D acquisition time (TAD),
                                                                                    //and right justify the result.
}

//BCD Digit        0     1    2    3   4    5    6    7    8    9
char array[10] = {0xFF,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};               //array for the common anode 7 seg display hex values

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];
}

void Display_Upper_Digit(char digit)
{
    PORTB = array[digit];
}

void OFF_Lower_Digit()
{
    PORTD = (PORTD & 0x80) | 0X7F;                                                  //turns off all except last bit
}

void OFF_Upper_Digit()
{
    PORTB = (PORTB & 0x80) | 0X7F;                                                  //turns off all except last bit
}

void Activate_Buzzer()                                                              //Function to Activate Buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
} 

void Deactivate_Buzzer(void)                                                        //Function to Deactivate Buzzer
{
    CCP2CON = 0x0;
    PORTCbits.RC1 = 0;
}

void Wait_One_Second_With_Beep()
{
    SEC_LED = 1;                                                                    // First, turn on the SEC LED
    Activate_Buzzer();                                                              // Activate the buzzer
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                                                    // then turn off the SEC LED
    Deactivate_Buzzer ();                                                           // Deactivate the buzzer
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
} 


void PED_Control(char Direction, char Num_Sec)
{
    if(Direction == 0)                                                              // If Direction is 0, turns off lower digit and displays
    {                                                                               // the countdown on the upper digit with beeps every second
        OFF_Lower_Digit();
        for(int i = Num_Sec-1; i >=0; i--)
        {
        Display_Upper_Digit(i);
        Wait_One_Second_With_Beep();
        }
    }
    if(Direction == 1)                                                              // If Direction is 1, turns off upper digit and displays
    {                                                                               // the countdown on the lower digit with beeps every second
        OFF_Upper_Digit();
        for(int j = Num_Sec-1; j >= 0; j--)
        {
        Display_Lower_Digit(j);
        Wait_One_Second_With_Beep();
        }

    }
}

void Night_Time_Mode(void)
{
     MODE_LED = 0;                                                                  //Turn off mode led
     OFF_Lower_Digit();                                                             //Turn off Lower digit 7 seg
     OFF_Upper_Digit();                                                             //Turn off Upper digit 7 seg
     SET_NS(RED);
     SET_EW(GREEN);
     SET_NSLT(RED);
     SET_EWLT(RED);
   
    Wait_N_Seconds(6);                                                              //Starts with NS going from green to yellow to red
    SET_EW(YELLOW);
    Wait_N_Seconds(2);                           
    SET_EW(RED);                                 
   
    if(NSLT_SW == 1)                                                                //If NSLT switch is on then NSLT cycles through the colors
    {
        SET_NSLT(GREEN);
        Wait_N_Seconds(5);
        SET_NSLT(YELLOW);
        Wait_N_Seconds(2);
        SET_NSLT(RED);
    }
    SET_NS(GREEN);                                                                  //NS cycles through the colors
    Wait_N_Seconds(7);
    SET_NS(YELLOW);
    Wait_N_Seconds(2);
    SET_NS(RED);

    if(EWLT_SW == 1)                                                                //If EWLT switch is on then EWLT cycles through the colors
    {
        SET_EWLT(GREEN);
        Wait_N_Seconds(7);
        SET_EWLT(YELLOW);
        Wait_N_Seconds(2);
        SET_EWLT(RED);
    }  
}

void Day_Time_Mode(void)
{
     MODE_LED = 1;                                                                  //Turn on mode LED
     SET_NS(RED);                                                                   
     SET_EW(GREEN);                                                                 //Starts with EW going from green to yellow to red
     SET_NSLT(RED);
     SET_EWLT(RED);
     if(EWPED_SW == 1)                                                              //If EWPED is on call PED function
     {
         PED_Control(0,7);                                                          
     }
     Wait_N_Seconds(6);
     SET_EW(YELLOW);
     Wait_N_Seconds(2);                           
     SET_EW(RED);                                 
     if(NSLT_SW == 1)                                                               //If NSLT switch is on then NSLT cycles through the colors
     {
         SET_NSLT(GREEN);
         Wait_N_Seconds(6);
         SET_NSLT(YELLOW);
         Wait_N_Seconds(2);
         SET_NSLT(RED);
     }
     SET_NS(GREEN);                                                                 //NS Cycles Through the colors
     if(NSPED_SW == 1)                                                              //If NSPED is on call PED function
     {
        PED_Control(1,8);
     }
     Wait_N_Seconds(8);
     SET_NS(YELLOW);
     Wait_N_Seconds(2);
     SET_NS(RED);
     if(EWLT_SW == 1)                                                              //If EWLT switch is on then EWLT cycles through the colors
     {
         SET_EWLT(GREEN);
         Wait_N_Seconds(8);
         SET_EWLT(YELLOW);
         Wait_N_Seconds(2);
         SET_EWLT(RED);
     }
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
   
    TRISA = 0x1F;                                                                   //Set first 4 bits to inputs
    TRISB = 0x00;                                                                   //Set to output
    TRISC = 0x00;                                                                   //Set to output
    TRISD = 0x00;                                                                   //Set to output
    TRISE = 0x00;                                                                   //Set to output
    
    Deactivate_Buzzer();
    OFF_Lower_Digit();
    OFF_Upper_Digit();
    
    while(1)
    {
        Set_ADCON0(0);                                                              //Select Channel AN0
        int num_step = Get_Full_ADC();                                              //Retrieves ADC step number at AN0
        float voltage_mv = num_step*4.88;                                           //Calculates voltage in mV from the ADC step number
        float AN0_voltage = voltage_mv/1000.0;                                      //Converts AN0 voltage from mV to V
       
        printf("voltage = %f V\r\n",AN0_voltage);                                   //Print to TeraTerm
       
        if(AN0_voltage < 2.5)                                                       // when <2.5 V, Day mode
        {
            Day_Time_Mode();
        }
        if(AN0_voltage >= 2.5)                                                      // when >=2.5 V, Night mode
        {
            Night_Time_Mode();
        }
    }
}
