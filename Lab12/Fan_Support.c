#include <p18f4620.h>
#include "Fan_Support.h"
#include "stdio.h"

extern char FAN;
extern char duty_cycle;

int get_RPM()
{
 int RPS = TMR3L / 2; // read the count. Since there are 2 pulses per rev
 // then RPS = count /2
TMR3L = 0; // clear out the count
return (RPS * 60); // return RPM = 60 * RPS
}

void do_update_pwm(char duty_cycle)
{
float dc_f;
 int dc_I;
PR2 = 0b00000100 ; // set the frequency for 25 Khz
T2CON = 0b00000111 ; //
dc_f = ( 4.0 * duty_cycle / 20.0) ; // calculate factor of duty cycle versus a 25 Khz
 // signal
dc_I = (int) dc_f; // get the integer part
 if (dc_I > duty_cycle) dc_I++; // round up function
 CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
 CCPR1L = (dc_I) >> 2;
}

void Toggle_Fan()
{
    if(FAN == 0) Turn_On_Fan();             // Turn on the fan if FAN is 0      
    else Turn_Off_Fan();                    // else turn off fan
}

void Turn_Off_Fan()
{
    FAN = 0;                                // Set fan to 0
    FAN_EN = 0;                             // Turn off the fan
    FAN_EN_LED = 0;                         // turn off fan LED
}

void Turn_On_Fan()
{
    FAN = 1;                                // set fan to 1
    do_update_pwm(duty_cycle);              // update pwm
    FAN_EN = 1;                             // turn on the fan
    FAN_EN_LED = 1;                         // turn on the fan LED
}

void Increase_Speed()
{
    if (duty_cycle == 100) {                // If duty cycle reaches 100, call 2 beeps instead of 1
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);          // Then update the pwm
    }
    else{                                   // else increment duty cycle by 5 and update the pwm
        duty_cycle += 5;
        do_update_pwm(duty_cycle);
    }


}

void Decrease_Speed()
{
    if (duty_cycle == 0) {                  // if duty cycle is 0 call 2 beeps instead of 1 
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);          // then update the pwn
    }
    else {                                  // else decrement the duty cycle by 5 and update the pwm
        duty_cycle -= 5;            
        do_update_pwm(duty_cycle);
    }
}

void Set_DC_RGB(char duty_cycle)            // Sets the RGB for the duty cycle
{
    int color[7] = {0x00,0x20,0x10,0x30,0x08,0x28,0x18};       // Array that contains values that will force RGB D1 into a color
    if (duty_cycle >= 70) {PORTB = 0x38;}
    else {PORTB = color[duty_cycle/10];}
}

void Set_RPM_RGB(int rpm)                   // Sets the RGB for the rpm
{
    int color[6] = {0x01,0x02,0x03,0x04,0x05,0x06};       // Array that contains values that will force RGB D2 into a color
    if (rpm == 0) {PORTD = (0x00 << 1) | PORTDbits.RD4 << 4 | PORTDbits.RD5 << 5 | PORTDbits.RD6 << 6;}
    else if (rpm >= 3000) {PORTD = (0x07 << 1) | PORTDbits.RD4 << 4 | PORTDbits.RD5 << 5 | PORTDbits.RD6 << 6;}
    else {PORTD = (color[rpm/500] << 1) | PORTDbits.RD4 << 4 | PORTDbits.RD5 << 5 | PORTDbits.RD6 << 6;}
}


