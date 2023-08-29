
#define _XTAL_FREQ      8000000

#define ACK             1
#define NAK             0

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02


#define FANEN_LED       PORTAbits.RA4
#define FAN_EN          PORTAbits.RA5
#define FAN_PWM         PORTCbits.RC2
#define RTC_ALARM_NOT   PORTBbits.RB2
#define KEY_PRESSED     PORTAbits.RA3
#define Setup_Time_Key        	12
#define Setup_Alarm_Key       	13
#define Setup_Fan_Temp_Key    	14  
#define Toggle_Fan_Key      	5
#define Prev            		3
#define Next            		4
#define Minus           		6
#define Plus            		7
#define Ch_Plus            		2
#define Ch_Minus				0



