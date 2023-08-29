#include<P18F4620.inc>
    config  OSC = INTIO67
    config  WDT = OFF
    config  LVP = OFF
    config  BOREN = OFF
   
    ORG    0x0000
 
    ;BCD IS GREEN
    ;OR IS PURPLE
    ;XOR IS BLUE
    ;ADD IS RED
    ;COMP IS OFF
    
START:
    InA    equ  0x25
    InB	    equ 0x29
    Result  equ  0x27

    MOVLW   0x0F	;Load W with 0x0F
    MOVWF   ADCON1	;Stores W into ADCON1 and makes it digital mode
    MOVLW   0xFF	;Load W with 0xFF
    MOVWF   TRISA	;Stores W into PORT A which makes it an input
    MOVLW   0xFF	;Load W with 0xFF
    MOVWF   TRISB	;Stores W into PORT B which makes it an input
    MOVLW   0x00	;Load W with 0x00
    MOVWF   TRISC	;Stores W into PORT C which makes it an output
    MOVLW   0x07	;Load W with 0x07
    MOVWF   TRISD	;Stores W into PORT D which makes it a mix of inputs and outputs
    MOVLW   0x00	;Load W with 0x00
    MOVWF   TRISE	;Stores W into PORT E which makes it an output

MAIN_LOOP:
    MOVF    PORTD,0	    ;Read from PORTA and stores it into W (d = 0)
    ANDLW   0x0F	    ;And W with the mask of 0x0F
   
    BTFSC   PORTD,2	    ;Bit 2, of PORTD, will act as a test bit.
    GOTO    PORTD1xx	;If test bit is 1, then bit 2 of PORTD is assigned as 1
    GOTO    PORTD0xx	;If test bit is 0, then bit 2 of PORTD is assigned as 0
   
PORTD1xx:
    GOTO    TASK_BCD	;Goes to TASK_BCD to execute BCD operation
   
PORTD0xx:  
    BTFSC   PORTD,1	    ;Bit 1, of PORTD, will act as a test bit.
    GOTO    PORTD01x	;If test bit is 1, then bit 1 of PORTD is assigned as 1
    GOTO    PORTD00x	;If test bit is 0, then bit 1 of PORTD is assigned as 0
   
PORTD01x:  
    BTFSC   PORTD,0	    ;Bit 0, of PORTD, will act as a test bit.
    GOTO    PORTD011	;If test bit is 1, then bit 0 of PORTD is assigned as 1
    GOTO    PORTD010	;If test bit is 0, then bit 0 of PORTD is assigned as 0
   
PORTD00x:   
    BTFSC   PORTD,0	    ;Bit 0, of PORTD, will act as a test bit.
    GOTO    PORTD001	;If test bit is 1, then bit 0 of PORTD is assigned as 1
    GOTO    PORTD000	;If test bit is 0, then bit 0 of PORTD is assigned as 0
   
PORTD010:   
    GOTO    TASK_XOR	;Goes to TASK_XOR to execute XOR operation
   
PORTD011:   
    GOTO    TASK_OR	    ;Goes to TASK_AND to execute AND operation

PORTD001:   
    GOTO    TASK_ADD	;Goes to TASK_ADD to execute ADD operation
   
PORTD000:   
    GOTO    TASK_COMP   ;Goes to TASK_COMP to execute 1's COMP operation

TASK_COMP:  
    BCF    PORTD,5	    ;Clears Bit 5
    BCF    PORTD,4	    ;Clears Bit 4
    BCF    PORTD,3	    ;Clears Bit 3
    CALL   SUB_COMP     ;Calls 1's COMP function
    GOTO   MAIN_LOOP    ;Goes back to MAIN_LOOP
   
TASK_ADD:  
    BCF    PORTD,5	    ;Clears Bit 5
    BCF    PORTD,4	    ;Clears Bit 4
    BSF    PORTD,3	    ;Clears Bit 3
    CALL   SUB_ADD      ;Calls ADD function
    GOTO   MAIN_LOOP	;Goes back to MAIN_LOOP
   
TASK_XOR:   
    BCF    PORTD,5	    ;Clears Bit 5
    BSF    PORTD,4	    ;Clears Bit 4
    BCF    PORTD,3	    ;Clears Bit 3
    CALL   SUB_XOR      ;Calls AND function
    GOTO   MAIN_LOOP    ;Goes back to MAIN_LOOP
   
TASK_OR:   
    BCF    PORTD,5	    ;Clears Bit 5
    BSF    PORTD,4	    ;Clears Bit 4
    BSF    PORTD,3	    ;Clears Bit 3
    CALL   SUB_OR       ;Calls XOR function
    GOTO   MAIN_LOOP    ;Goes back to MAIN_LOOP
       
TASK_BCD:
    BSF    PORTD,5	    ;Clears Bit 5
    BCF    PORTD,4	    ;Clears Bit 4
    BCF    PORTD,3	    ;Clears Bit 3
    CALL   SUB_BCD      ;Calls BCD function
    GOTO   MAIN_LOOP    ;Goes back to MAIN_LOOP
    
SUB_COMP:
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    COMF	InA, 0		    ;Take 1's comp and store in W
    ANDLW	0x0F		    ;And W with the mask of 0x0F 
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    BZ		BRANCH1		    ;Jump to BRANCH1 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    
BRANCH1:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    RETURN

SUB_ADD:
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    MOVF	PORTB,0		    ;Read from PORTB and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InB		        ;Move W to InB  
    ADDWF	InA,0		    ;Add InA to W
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    BZ		BRANCH2		    ;Jump to BRANCH2 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    
BRANCH2:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    RETURN

SUB_XOR:
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    MOVF	PORTB,0		    ;Read from PORTB and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InB		        ;Move W to InB  
    XORWF	InA,0		    ;XOR InA with W   
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    BZ		BRANCH3		    ;Jump to BRANCH3 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    
BRANCH3:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    RETURN

SUB_OR:
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    MOVF	PORTB,0		    ;Read from PORTB and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InB		        ;Move W to InB  
    IORWF	InA,0		    ;OR InA with W
    MOVWF	Result		    ;Move W to result  
    MOVFF	Result,PORTC	;Move W to portC  
    BZ		BRANCH4		    ;Jump to BRANCH4 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    
BRANCH4:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    RETURN
    
SUB_BCD: 
    MOVF	PORTA,0		    ;Read from PORTA and stores it into W
    ANDLW	0x0F		    ;And W with the mask of 0x0F   
    MOVWF	InA		        ;Move W to InA  
    MOVLW	0x09		    ;Load W with 0x09
    CPFSGT	InA, 0		    ;If InA is greater than W goto ADD6, else go to DISPLAY
    GOTO	DISPLAY
    GOTO	ADD6
    
ADD6:
    MOVLW	0x06		    ;Load W with 0x06
    ADDWF	InA,0		    ;Add 0x06 to InA
    MOVWF	Result		    ;Move W to result
    MOVFF	Result,PORTC	;Move RESULT to portC
    BZ		BRANCH5		    ;Jump to BRANCH5 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    RETURN
    
DISPLAY:
    MOVF	InA,0		    ;Move W to InA
    MOVFF	InA,PORTC	    ;Move RESULT to portC 
    BZ		BRANCH5		    ;Jump to BRANCH5 label if Z flag is set
    BCF		PORTE, 2	    ;If Z flag is not set, clear PORTE bit 2 to 0
    RETURN
    
BRANCH5:
    BSF		PORTE, 2	    ;If Z flag is set, set PORTE bit 2 to 1
    RETURN
    
END
    
    