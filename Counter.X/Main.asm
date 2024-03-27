;-----------------------------
; Title: Seven Segment Counter
;-----------------------------
; Purpose: Takes input from a keypad and either counts up or down 
; Dependencies: None
; Compiler: MPLAB X IDE v6.20
; Author: Tyler Walters
; OUTPUTS: Outputs Connected to seven segment display
; INPUTS: Connected to keypad
; Versions:
;  	V1.0: 3/24/2024 -First Version
;-----------------------------
#include <xc.inc>
#include "AssemblyConfig.inc"
PSECT absdata,abs,ovrld
;----------------
; PROGRAM INPUTS
;----------------
	
LARGE_LOOP EQU	0XFF
SMALL_LOOP EQU	0XFF

    
    
;----------------
; REGISTERS
;----------------
BUTTON	EQU	0X10
DELAY1	EQU	0X11
DELAY2	EQU	0X12
DELAY3	EQU	0X13
LAST_CODE_ADDR	EQU 0X14
FIRST_CODE_ADDR	EQU 0X15
;----------------
; PROGRAM OUTPUTS
;----------------
BANKSEL	ANSELA
CLRF ANSELA
BANKSEL	ANSELB
CLRF ANSELB
BANKSEL	ANSELC
CLRF ANSELD
BANKSEL TRISA
SETF TRISA
CLRF TRISB  ; OUTPUT FOR KEYPAD SCANNING
CLRF TRISD  ; OUTPUT GOING TO 7 SEGMENT
MOVLW 0X20
MOVWF TBLPTRL
MOVLW 0XC0
MOVWF PORTD
GOTO	START
org 0x20
db  0xC0, 0xF9, 0xA4, 0xB0, 0x99 ; numbers 0-4
db  0x92, 0x82, 0xF8, 0x80, 0x98 ; numbers 5-9
db  0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e ; Letters A-F

;----------------
; MAIN PROGRAM
;----------------
	ORG 0X30
START: 
    MOVLW   0X20
    MOVWF   FIRST_CODE_ADDR
    MOVLW   0X2F
    MOVWF   LAST_CODE_ADDR
    CLRF    WREG
    CALL    SCAN    ;Scan keyboard
    CALL    DISPLAY ;Figure out what to do display with user input
    CALL    DELAY   ; Delay so 7 segment doesn't move too fast
    GOTO    START
	
;--------------
;SUBROUTINES
;--------------
SCAN: 
    CLRF WREG
    BSF	PORTB, 0    ;SET 1ST COLUMN HIGH
    BTFSC PORTA, 0  ; CHECK IF ROW ONE ACTIVATED
    MOVLW 0X1	    ; IF IT DID 1 IS PRESSED. LOAD ONE INTO WREG
    BCF PORTB,0	    ; CLEAR AND CHECK NEXT COLUMN
		    ; WERE ONLY CONCERNED WITH THE FIRST ROW IN THIS PROGRAM
    BSF PORTB, 1    
    BTFSC PORTA, 0  
    MOVLW 0X2	    
    BCF	PORTB, 1
    
    BSF PORTB, 2
    BTFSC PORTA, 0  
    MOVLW 0X3	    
    BCF	PORTB, 2
    
    RETURN
;---------- END OF SCAN
    
    
DISPLAY:
UP_DOWN_RESET:
    MOVWF BUTTON    
    MOVLW 0X0	    ;First Check for 0 or no input
    CPFSEQ BUTTON   ;If zero return to main
    GOTO NONZERO    ;If not zero check what was input
    RETURN
NONZERO:
    MOVLW 0X2
    CPFSGT  BUTTON	;Check if greater than 2
    GOTO LTEQ		;If not check for less than equal to
    GOTO COUNT_RESET	;if greater than reset counter
    
LTEQ:
    CPFSLT BUTTON	
    GOTO COUNT_DOWN	;input equals 2 count down
    GOTO COUNT_UP	;input equals 1 count up
    
COUNT_UP:
    CLRF WREG		    
    MOVFF TBLPTRL, WREG	    
    CPFSEQ LAST_CODE_ADDR   ;See if counter needs to flip from 9 to 0
    GOTO AFTER_RESET_UP	    
    GOTO ZERO_COUNTER_UP    
    CLRF WREG
    AFTER_RESET_UP:
    TBLRD+*		    ;Preincrement and display data at tblptr
    MOVFF TABLAT, PORTD
    RETURN
    
ZERO_COUNTER_UP:
    MOVLW 0X1F
    MOVWF TBLPTRL	;Reset the tbptr to 1 before 0 position
    GOTO AFTER_RESET_UP	    

COUNT_DOWN:
    CLRF WREG	    
    MOVFF TBLPTRL, WREG	    
    CPFSEQ  FIRST_CODE_ADDR	;See if tblptr needs to reset from 0 to 9
    GOTO AFTER_RESET_DOWN
    GOTO RESET_COUNT_DOWN
    CLRF WREG
    AFTER_RESET_DOWN:
    DECF TBLPTRL		;Predecrement before displaying data
    TBLRD*
    MOVFF TABLAT, PORTD
    RETURN
    
RESET_COUNT_DOWN:
    MOVLW   0X30
    MOVWF   TBLPTRL		;Move tblptr to 9 address
    GOTO    AFTER_RESET_DOWN
    
    
 COUNT_RESET:
    MOVLW 0X20
    MOVWF TBLPTRL   
    TBLRD*		    
    MOVFF TABLAT, PORTD		;Move tblptr address to zero then send to portd
    RETURN
    
;----------------- END OF DISPLAY ROUTINE
    
DELAY:
    CLRF WREG
    MOVLW LARGE_LOOP	    ;Create 3 loops which can be adjusted with loop variables
    MOVWF DELAY1
    MOVWF DELAY2
    MOVLW SMALL_LOOP
    MOVWF DELAY3
LOOP3:	
    NOP
    DCFSNZ DELAY1
    BRA	LOOP3
LOOP2:
    MOVLW LARGE_LOOP
    MOVWF DELAY1
    DECFSZ DELAY2
    BRA LOOP3
lOOP1:
    MOVLW LARGE_LOOP
    MOVWF DELAY2
    DECFSZ DELAY3
    BRA	LOOP2
    RETURN