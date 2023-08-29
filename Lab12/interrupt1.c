
#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"

unsigned char bit_count;
unsigned int Time_Elapsed;

unsigned long long Nec_code;
unsigned char Nec_state = 0;

extern char Nec_Button;
extern short Nec_OK;


void Init_Interrupt()
{
    INTCON3bits.INT1IF = 0;                 // Clear external interrupt INT1IF
    INTCON2bits.INTEDG1 = 0;                // Edge programming for INT 1 falling edge H to L
    INTCON3bits.INT1IE = 1;                 // Enable external interrupt INT1IE

    TMR1H = 0;                              // Reset Timer1
    TMR1L = 0;                              
    PIR1bits.TMR1IF = 0;                    // Clear timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;                    // Enable Timer 1 interrupt
    INTCONbits.PEIE = 1;                    // Enable Peripheral interrupt
    INTCONbits.GIE = 1;                     // Enable global interrupts
}

void Enable_INT_Interrupt()
{
    INTCON3bits.INT1IE = 1;          		// Enable external interrupt
    INTCON2bits.INTEDG1 = 0;        		// Edge programming for INT1 falling edge
}    

void interrupt high_priority chkisr()
{
    if (PIR1bits.TMR1IF == 1) TIMER1_isr();
    if (INTCON3bits.INT1IF == 1) INT1_isr();
}

void TIMER1_isr(void)
{
    Nec_state = 0;                          // Reset decoding process
    INTCON2bits.INTEDG1 = 0;                // Edge programming for INT 1 falling edge
    T1CONbits.TMR1ON = 0;                   // Disable T1 Timer
    PIR1bits.TMR1IF = 0;                    // Clear interrupt flag
}

void Reset_Nec_State()
{
    Nec_state=0;
    T1CONbits.TMR1ON = 0;
}

void INT1_isr(void)
{
    INTCON3bits.INT1IF = 0;                     // Clear external interrupt INT1IF
    if (Nec_state != 0)
    {
        Time_Elapsed = (TMR1H << 8) | TMR1L;    // Store Timer1 value
        TMR1H = 0;                              // Reset Timer1
        TMR1L = 0;
    }
    
    switch(Nec_state)
    {
        case 0 :
        {
                                            // Clear Timer 1
            TMR1H = 0;                      // Reset Timer1
            TMR1L = 0;                      //
            PIR1bits.TMR1IF = 0;            //
            T1CON= 0x90;                    // Program Timer1 mode with count = 1usec using System clock running at 8Mhz
            T1CONbits.TMR1ON = 1;           // Enable Timer 1
            bit_count = 0;                  // Force bit count (bit_count) to 0
            Nec_code = 0;                   // Set Nec_code = 0
            Nec_state = 1;                  // Set Nec_State to state 1
            INTCON2bits.INTEDG1 = 1;        // Change Edge interrupt of INT 1 to Low to High            
            return;
        }
        
        case 1 :
        {
            if (Time_Elapsed<9500 && Time_Elapsed>8500)                             // If Time_Elapsed value read is between 8500 usec and 9500 usec,
            {
                Nec_state=2;                                                        // then force variable Nec_state to state 2
            }
            else Reset_Nec_State();                                                 // Else, call function Reset_Nec_State()  
            INTCON2bits.INTEDG1 = 0;                                                // Change Edge programming for INT0 falling edge from High to Low
            return;          
        }
        
        case 2 :                            
        {
            if (Time_Elapsed<5000 && Time_Elapsed>4000)                             // If Time_Elapsed value is between 4000 usec and 5000 usec,
                Nec_state=3;                                                        // then force Nec_state to state 3  
            else Reset_Nec_State();                                                 // Else, call function Reset_Nec_State()  
           
            INTCON2bits.INTEDG1 = 1;                                                // Change Edge interrupt of INT 1 to Low to High          
            return;                                                                 
        }
        
        case 3 :                            // Add your code here
        {
            if (Time_Elapsed<700 && Time_Elapsed>400)                               // If Time_Elapsed value is between 400 usec and 700 usec,
                Nec_state=4;                                                        // then force Nec_state to state 4  
            else Reset_Nec_State();                                                 // Else, call function Reset_Nec_State()  
            
            INTCON2bits.INTEDG1 = 0;                                                // Change Edge programming for INT1 falling edge from High to Low
            return;                                                                 
        }
        
        case 4 :                            // Add your code here
        {
            if (Time_Elapsed<1800 && Time_Elapsed>400)                              // If Time_Elapsed value is between 400 usec and 1800 usec,
                 {
                    Nec_code=Nec_code<<1;                                           // Shift left Nec_code by 1 bit
                   
                    if (Time_Elapsed>1000)                                          // Check if Time_Elapsed is greater than 1000 usec,
                    {
                        Nec_code=Nec_code+1;                                        // then add 1 to Nec_code
                    }
                                                                                    
                   
                    bit_count=bit_count+1;                                          // Increment variable bit_count by 1
                   
                    if(bit_count>31)                                                // Check if bit_count > 31,
                    {
                        Nec_OK=1;                                                   // then set nec_ok flag to 1
                        INT1IE=0;                                                   // set INT1IE = 0
                        Nec_Button=(char)(Nec_code>>8);
                        Nec_state=0;                                                // set Nec_state = 0 (state 0)  
                    }
                    else
                        Nec_state=3;                                                // Set Nec_state to state 3
                 }        
                 
                 else Reset_Nec_State();                                            // If Time_Elapsed value is not between 400 usec and 1800 usec,
                                                                                    // call function Reset_Nec_State()
                INTCON2bits.INTEDG1 = 1;                                            // Change Edge programming for INT0 falling edge Low to High
                return;                                                             // Return results              
        }
    }
}
