/*-----------------------------
; Title: Seven Segment Calculator
;-----------------------------
; Purpose: Takes two numbers and operation from keypad, performs them then outputs 
; to two seven segment displays  
; Dependencies: None
; Compiler: MPLAB X IDE v6.20
; Author: Tyler Walters
; OUTPUTS: Outputs Connected to two seven segment display
; INPUTS: Connected to keypad
 Versions:
	V1.0: 4/8/2024 -First Version
 */
#define _XTAL_FREQ 8000000  // Define your PIC18 clock frequency (e.g., 8 MHz)

#include <xc.h>
#include <stdlib.h>
#include "Config.h"

signed char X_Input_REG;
signed char Y_Input_REG;
signed char Operation_REG;
signed char calculation;
signed char Display_Result_REG;
// functions
signed char scan_number(char LED_Count);
signed char scan_operation();
signed int calculate(char x, char y, char op);
void PORT_Output(int answer);


void main(void) {
    TRISA = 0xFF;   // make PORTA input
    ANSELA = 0; //digital
    TRISB  = 0;     // port b output
    ANSELB = 0;
    ANSELD = 0;
    TRISD = 0;
    TRISC = 0;
    ANSELC = 0;
    PORTD = 0xFF;
    PORTC = 0xFF;
    
    while(1){
    X_Input_REG = 0;
    Y_Input_REG = 0;
    Operation_REG = 0;
    calculation = 0;
    char LED_Count = 1;
    Display_Result_REG = 0; // reset all values
    
    X_Input_REG = scan_number(LED_Count); // get first number
    Operation_REG = scan_operation();   // get operation
    LED_Count = 2;                      // tell function what LED to light
    Y_Input_REG = scan_number(LED_Count);   //Get second number
    calculation = calculate(X_Input_REG, Y_Input_REG, Operation_REG); // Perform calc
    PORT_Output(calculation);           // Convert and display result
    }
    return;
}

signed char scan_number(char LED_Count){
    signed char input = 10;
    signed char output = 0;
    signed char button = 0;
    signed char count = 2;
    
    while (input == 10) // no input for 10 therefore loop till its changed
    {
        PORTBbits.RB0 = 1; 
        if (PORTAbits.RA0 == 1)input = 1; // Scanning keypad for corresponding number press 
        if(PORTAbits.RA1 == 1) input = 4;
        if(PORTAbits.RA2 == 1) input = 7;
        PORTBbits.RB0 = 0;
        
         PORTBbits.RB1 = 1; 
        if (PORTAbits.RA0 == 1)input = 2;
        if(PORTAbits.RA1 == 1) input = 5;
        if(PORTAbits.RA2 == 1) input = 8;
        if(PORTAbits.RA3 == 1) input = 0;
        PORTBbits.RB1 = 0;
        
         PORTBbits.RB2 = 1; 
        if (PORTAbits.RA0 == 1)input = 3;
        if(PORTAbits.RA1 == 1) input = 6;
        if(PORTAbits.RA2 == 1) input = 9;
        PORTBbits.RB2 = 0;
       
}
   // __delay_ms(200); // debounce
    
    input *=10;     // first digit is 10 to the power 1
    output = input; 
  
    input = 10;
    while (input == 10) // get second digit
    {
        PORTBbits.RB0 = 1; 
        if (PORTAbits.RA0 == 1)input = 1;
        if(PORTAbits.RA1 == 1) input = 4;
        if(PORTAbits.RA2 == 1) input = 7;
        PORTBbits.RB0 = 0;
        
         PORTBbits.RB1 = 1; 
        if(PORTAbits.RA0 == 1) input = 2;
        if(PORTAbits.RA1 == 1) input = 5;
        if(PORTAbits.RA2 == 1) input = 8;
        if(PORTAbits.RA3 == 1) input = 0;
        PORTBbits.RB1 = 0;
        
         PORTBbits.RB2 = 1; 
        if (PORTAbits.RA0 == 1)input = 3;
        if(PORTAbits.RA1 == 1) input = 6;
        if(PORTAbits.RA2 == 1) input = 9;
        PORTBbits.RB2 = 0;
        
        PORTB = 0;
}
    output += input; 
    //__delay_ms(200);
    if(LED_Count == 1 ){
            PORTBbits.RB5 = 1;
            PORTCbits.RC7 = 0;
    }
    if(LED_Count == 2){
            PORTCbits.RC7 = 1;
            PORTBbits.RB6 = 1;
            PORTCbits.RC6 = 0;
    }
    return output;
}       

signed char scan_operation(){
    signed char op_key = 0;                 // no op for 0, scan till variable changed
    while (op_key ==0) {
     
     PORTBbits.RB3 = 1; 
        if(PORTAbits.RA0 == 1) op_key = 1;      // scan abcd only
        if(PORTAbits.RA1 == 1) op_key = 2;
        if(PORTAbits.RA2 == 1) op_key = 3;
        if(PORTAbits.RA3 == 1) op_key = 4; 
    }
       // __delay_ms(200);
        return op_key;
}

signed int calculate(char x, char y, char op){
    signed char output = 0; // take in op variable and calc based on that
    if(op == 1)
        output = x + y;
    if(op == 2)
        output = x-y;
    if(op == 3)
        output = x*y;
    if(op == 4)
        output = x/y; 
    return output;
}

void PORT_Output(signed int answer){
    signed char first_dig = 0;
    signed char second_dig = 0;
    signed char enter = 0;
    int i = 0;
    char display1 = 0;
    char display2 = 0;
    
    if(answer<0){
        answer*= -1;
        PORTCbits.RC7 = 0;
    }                           // turn on dot if number is negative
    first_dig = answer/10;      // get first digit
    second_dig = answer % 10;   // get second digit
    
    if(first_dig == 0) display1 = 0xC0;     //table for converting to 7 segment 
    else if(first_dig == 1) display1 = 0xF9;
    else if(first_dig == 2) display1 = 0xA4;
    else if(first_dig == 3) display1 = 0xB0;
    else if(first_dig == 4) display1 = 0x99;
    else if(first_dig == 5) display1 = 0x92;
    else if(first_dig == 6) display1 = 0x82;
    else if(first_dig == 7) display1 = 0xF8;
    else if(first_dig == 8) display1 = 0x80;
    else if(first_dig == 9) display1 = 0x98;
    
    if(second_dig == 0) display2 = 0xC0;
    else if(second_dig == 1) display2 = 0xF9;
    else if(second_dig == 2) display2 = 0xA4;
    else if(second_dig == 3) display2 = 0xB0;
    else if(second_dig == 4) display2 = 0x99;
    else if(second_dig == 5) display2 = 0x92;
    else if(second_dig == 6) display2 = 0x82;
    else if(second_dig == 7) display2 = 0xF8;
    else if(second_dig == 8) display2 = 0x80;
    else if(second_dig == 9) display2 = 0x98;
    
    while(enter == 0){          // scan while waiting for # key 
        PORTBbits.RB2 = 1;
        if(PORTAbits.RA3 == 1) enter =1;
 
    }
    PORTCbits.RC6 = 1;      // turn off second LED lit after second input 
    for(i = 1; i<1000; i++) // toggle between two 7 Segments fast enough to display both
    {
        PORTBbits.RB5 = 1;
        PORTD = display1;
        __delay_ms(1);
        PORTBbits.RB5 = 0;
        PORTBbits.RB6 = 1;
        PORTD = display2; 
        __delay_ms(1);
        PORTBbits.RB6 = 0;
        
    }
    PORTD = 0xFF; // turn off 7 segment 
    PORTB = 0;     // turn off any scanning 
    return;
}