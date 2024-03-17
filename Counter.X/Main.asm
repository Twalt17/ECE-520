;-----------------------------
; Title: Temperature Control Systen
;-----------------------------
; Purpose: Turns on heating or cooling systems based on desired temperature 
; Dependencies: None
; Compiler: MPLAB X IDE v6.20
; Author: Tyler Walters
; OUTPUTS: Outputs Connected to heating and cooling systems
; INPUTS: Connected to keypad and temperature sensor 
; Versions:
;  	V1.0: 3/2/2024 -First Version
;-----------------------------
#include <xc.inc>
#include <./AssemblyConfig.inc>
;----------------
; PROGRAM INPUTS
;----------------
SETF TRISA    
    
    
;----------------
; REGISTERS
;----------------
BUTTON	EQU	0X10
;----------------
; PROGRAM OUTPUTS
;----------------
CLRF TRISB  ; OUTPUT FOR KEYPAD SCANNING
CLRF TRISC  ; OUTPUT GOING TO 7 SEGMENT
org 0x20
db  0xC0, 0xF9, 0xA4, 0xB0, 0x99 ; numbers 0-4
db  0x92, 0x82, 0xF8, 0x80, 0x98 ; numbers 5-9

;----------------
; MAIN PROGRAM
;----------------
	ORG 0X30
START: 
    
	
;--------------
;SUBROUTINES
;--------------
SCAN: 
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
    MOVLW 0X2	    
    BCF	PORTB, 2
    
    RETURN
;---------- END OF SCAN
    
    
DISPLAY:
UP_DOWN_RESET:
    MOVWF BUTTON
    MOVLW 0X2
    CPFSGT  BUTTON
    GOTO GTEQ
    GOTO COUNT_UP
    
GTEQ:
    CPFSLT BUTTON
    GOTO COUNT_DOWN
    GOTO COUNT_RESET
    
COUNT_UP:
    CLRF WREG
    MOVFF TBLPTRL, WREG
    CPFSLT 0X2A
    GOTO ZERO_COUNTER_UP
    CLRF WREG
    AFTER_RESET_UP:
    TBLRD*+
    MOVFF TABALAT, PORTC
    RETURN
    
ZERO_COUNTER_UP:
    MOVLW OX20
    MOVWF TBLPTRL
    GOTO AFTER_RESET_UP

COUNT_DOWN:
    CLRF WREG
    MOVFF TBLPTRL, WREG
    CPFSGT  0X1F
    GOTO RESET_COUNT_DOWN
    CLRF WREG
    AFTER_RESET_UP:
    TBLRD*-
    MOVFF TABALAT, PORTC
    RETURN
    
RESET_COUNT_DOWN:
    MOVLW OX28
    MOVWF TBLPTRL
    GOTO AFTER_RESET_DOWN
    
    
 COUNT_RESET:
    MOVLW 0X20
    MOVWF TBLPTRL
    TBLRD*
    MOVFF TABALAT, PORTC
    
;----------------- END OF DISPLAY ROUTINE