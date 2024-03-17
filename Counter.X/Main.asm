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
;  	V1.0: 3/17/2024 -First Version
;-----------------------------
#include <xc.inc>
#include "AssemblyConfig.inc"
;----------------
; PROGRAM INPUTS
;----------------
SETF TRISA    
LARGE_LOOP EQU	0XFF
SMALL_LOOP EQU	0X7

    
    
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
    MOVLW   0X1F
    MOVWF   FIRST_CODE_ADDR
    MOVLW   0X2A
    CLRF    WREG
    CALL    SCAN
    CALL    DISPLAY
    CALL    DELAY
    GOTO    START
	
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
    CPFSLT LAST_CODE_ADDR
    GOTO ZERO_COUNTER_UP
    CLRF WREG
    AFTER_RESET_UP:
    TBLRD*+
    MOVFF TABLAT, PORTC
    RETURN
    
ZERO_COUNTER_UP:
    MOVLW 0X20
    MOVWF TBLPTRL
    GOTO AFTER_RESET_UP

COUNT_DOWN:
    CLRF WREG
    MOVFF TBLPTRL, WREG
    CPFSGT  0X1F
    GOTO RESET_COUNT_DOWN
    CLRF WREG
    AFTER_RESET_DOWN:
    TBLRD*-
    MOVFF TABLAT, PORTC
    RETURN
    
RESET_COUNT_DOWN:
    MOVLW   0X20
    MOVWF   TBLPTRL
    GOTO    AFTER_RESET_DOWN
    
    
 COUNT_RESET:
    MOVLW 0X20
    MOVWF TBLPTRL
    TBLRD*
    MOVFF TABLAT, PORTC
    RETURN
    
;----------------- END OF DISPLAY ROUTINE
    
DELAY:
    CLRF WREG
    MOVLW LARGE_LOOP
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