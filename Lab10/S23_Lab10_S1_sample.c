#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>
#include "ST7735_TFT.h"
#include "Interrupt.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz


void Initialize_Screen();


short Nec_OK = 0;
char Nec_Button;


// colors
#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

#define Circle_Size     20              // Size of Circle for Light
#define Circle_X        60              // Location of Circle
#define Circle_Y        80              // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of Big Text


char buffer[31];                        // general buffer for display purpose
char *nbr;                              // general pointer used for buffer
char *txt;

char array1[21]={0xa2,0x62,0xe2,0x22,0x02,0xc2,0xe0,0xa8,                           // Array that includes all the codes of the buttons on the remote control
                 0x90, 0x68, 0x98,0xb0,0x30,0x18, 0x7a, 0x10, 0x38, 0x5a, 0x42,     // which have been placed consecutively based on the order of the buttons
                 0x4a, 0x52};                                                       // on the remote control.
char txt1[21][4] ={"CH-\0","CH \0","CH+\0","PRE\0","NEX\0","PLY\0"," - \0"," + \0", // Array that includes all the text values of the buttons on the remote control.
                   "EQ \0"," 0 \0","100\0","200\0"," 1 \0"," 2 \0"," 3 \0"," 4 \0",
                    " 5 \0"," 6 \0", " 7 \0"," 8 \0", " 9 \0" };

										// add more value into this array
int color[21]={RD,RD, RD, BU, BU,GR,MA,MA,MA,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK};  // Array that reflects the color of each button on the remote control.


int colord1[21] = {0x08,0x08,0x08,0xE0,0xE0,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};       // Array that contains values that will force RGB D1 into a color
int colord2[21]={0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x05,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};         // Array that contains values that will force RGB D2 into a color
int colord3[21]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07};         // Array that contains values that will force RGB D3 into a color


void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}
void Activate_Buzzer()                                                              // Function that activate buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;                                                       
}

void Deactivate_Buzzer()                                                            // Function that deactivates buzzer
{
    CCP2CON = 0x0;
	PORTCbits.RC1 = 0;
}

void Wait_1_Sec()                                                                   // Function that Waits 1 Second
{
    for(int j = 0;j < 0xFFFF;j++);                                                   
}
void Wait_One_Second_With_Beep(void)                                                // Function that waits one second with a beep from a buzzer
{
    Activate_Buzzer();                                                              // Activates Buzzer
    Wait_1_Sec();                                                                   // Waits one second
    Deactivate_Buzzer();                                                            // Deactivates Buzzer
}


void main()
{
    init_UART();
    OSCCON = 0x70;                          // 8 Mhz
    nRBPU = 0;                              // Enable PORTB internal pull up resistor
	TRISB = 0x07;							
    TRISC = 0x00;                           // PORTC as output
    TRISD = 0x00;                           // PORTD as output
    TRISE = 0x00;                           // PORTE as output
    ADCON1 = 0x0F;                          
    Initialize_Screen();					
	
    Init_Interrupt();
	
	
    TMR1H = 0;                              // Reset Timer1
    TMR1L = 0;                              //
    PIR1bits.TMR1IF = 0;                    // Clear timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;                    // Enable Timer 1 interrupt
    INTCONbits.PEIE = 1;                    // Enable Peripheral interrupt
    INTCONbits.GIE = 1;                     // Enable global interrupts
    Nec_OK = 0;                             // Clear flag
 
    
    while(1)
    {
        if (Nec_OK == 1)
        {
            Nec_OK = 0;
            printf ("NEC_Code = %x\r\n\n", Nec_Button);                             //Print hex value of button to teraterm 
            Enable_INT_Interrupt();
           

            char found = 0xff;
            
            for(int i=0;i<21;i++)                                                   
            {                                                                      
                if (Nec_Button==array1[i])                                           // If the retireved code Nec_Button is equal to the code in the array1[i]                                      
                {
                        found=i;                                                    // Initialize found to the value of i
                        break;                                                      // Break
                }
            }
            
            if (found != 0xff) 
            {
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]); 
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1);
				
				PORTB=colord1[found];                                                // Output to RGB D1 the color of the variable found that is the index of the array colord1
                PORTD = (colord2[found] << 1) | PORTDbits.RD4 << 4 | PORTDbits.RD5 << 5 | PORTDbits.RD6 << 6; // Output to RGB D2 the color of the variable found that is the index of the array colord2
                PORTE = colord3[found];                                              // Output to RGB D3 the color of the variable found that is the index of the array colord3 
                PORTCbits.RC0 = 1;                                                   //turn on flash led        
                Wait_One_Second_With_Beep();                                         //wait one second and play beep
                PORTCbits.RC0 = 0;                                                   //turn on flash led 
            }
        }
    }
}

void Initialize_Screen()
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L S23/S1/T2");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

