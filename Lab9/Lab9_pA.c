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

void Init_ADC()
{
    ADCON0 = 0*0x4 + 1;
    ADCON1= 0x0E;
    ADCON2= 0xA9;
}

void init_UART()
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

int INT0_flag, INT1_flag, INT2_flag = 0;

void Do_Init()              // Initialize the ports
{
    init_UART();            // Initialize the uart
    Init_ADC();             // Initiailize the ADC with the
                            // programming of ADCON1
    RBPU = 0;               // Make sure that PORTB has pull-up 
    OSCCON=0x70;            // Set oscillator to 8 MHz
    TRISB = 0x07;           // Configure the PORTB to make sure
                            // that all the INTx pins are
                            // inputs
    INTCONbits.INT0IF = 0 ; // Clear INT0IF
    INTCON3bits.INT1IF = 0; // Clear INT1IF
    INTCON3bits.INT2IF =0;  // Clear INT2IF
   
    INTCON2bits.INTEDG0=0 ; // INT0 EDGE falling
    INTCON2bits.INTEDG1=0;  // INT1 EDGE falling
    INTCON2bits.INTEDG2=1;  // INT2 EDGE rising

    INTCONbits.INT0IE =1;   // Set INT0 IE
    INTCON3bits.INT1IE=1;   // Set INT1 IE
    INTCON3bits.INT2IE=1;   // Set INT2 IE
   
    INTCONbits.GIE=1;       // Set the Global Interrupt Enable
}

void INT0_ISR()
{
    INTCONbits.INT0IF=0;                               // Clear the interrupt flag
    INT0_flag = 1;                                     // set software INT0_flag
}

void INT1_ISR()
{
    INTCON3bits.INT1IF=0;                               // Clear the interrupt flag
    INT1_flag = 1;                                      // set software INT1_flag
}

void INT2_ISR()
{
    INTCON3bits.INT2IF=0;                               // Clear the interrupt flag
    INT2_flag = 1;                                      // set software INT2_flag
}

void interrupt high_priority chkisr()
{
    if (INTCONbits.INT0IF == 1) INT0_ISR();             // check if INT0
                                                        // has occured
    if (INTCON3bits.INT1IF == 1) INT1_ISR();
    if (INTCON3bits.INT2IF == 1) INT2_ISR();
}

void main(void)
{
    Do_Init(); // Initialization
    
    while (1)
    { // Do nothing,
        if (INT0_flag == 1)
        {
            INT0_flag = 0;                                  // clear the flag
            printf ("INT0 interrupt pin detected \r\n");
                                                            // print a message that INT0 has
                                                            // occurred
        }
       
        if (INT1_flag == 1)
        {
            INT1_flag = 0;                                  // clear the flag
            printf ("INT1 interrupt pin detected \r\n");
                                                            // print a message that INT1 has
                                                            // occurred
        }
       
        if (INT2_flag == 1)
        {
            INT2_flag = 0;                                  // clear the flag
            printf ("INT2 interrupt pin detected \r\n");
                                                            // print a message that INT2 has
                                                            // occurred
        }
    }  
}
