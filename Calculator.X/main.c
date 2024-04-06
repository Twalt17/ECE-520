/*
 * File:   main.c
 * Author: Tyler
 *
 * Created on April 6, 2024, 11:05 AM
 */

#define _XTAL_FREQ 8000000  // Define your PIC18 clock frequency (e.g., 8 MHz)

#include <xc.h>
#include <stdlib.h>

unsigned char X_Input_REG;
unsigned char Y_Input_REG;
unsigned char Operation_REG;
unsigned char calculation;
unsigned char Display_Result_REG;
// functions
unsigned char scan_number();
unsigned char scan_operation();
unsigned char calculate(unsigned char x, unsigned char y, unsigned char op);
void PORT_Output(unsigned char answer);


void main(void) {
    TRISA = 0xFF;   // make RA0 input
    ANSELA = 0; //digital
    TRISB  = 0;     // port b output
    ANSELB = 0;
    ANSELD = 0;
    TRISD = 0;
    
    while(1){
    X_Input_REG = 0;
    Y_Input_REG = 0;
    Operation_REG = 0;
    calculation = 0;
    Display_Result_REG = 0;
    
    X_Input_REG = scan_number();
    Operation_REG = scan_operation();
    Y_Input_REG = scan_number();
    calculation = calculate(X_Input_REG, Y_Input_REG, Operation_REG);
    PORT_Output(calculation);
    }
    return;
}

unsigned char scan_number(){
    unsigned char input = 10;
    unsigned char output = 0;
    unsigned char button = 0;
    unsigned char count = 2;
    
    while (input == 10)
    {
        PORTBbits.RB0 = 1; 
        if (PORTAbits.RA0 == 1)input = 1;
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
    __delay_ms(1); // debounce
    
    input *=10;     // first digit is 10 to the power 1
    output = input; 
  
    input = 10;
    while (input == 10)
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
}
    output += input; 
    __delay_ms(1);
    return output;
}       

unsigned char scan_operation(){
    unsigned char op_key = 0;
    while (op_key ==0) {
     PORTBbits.RB0 = 1; 
        if(PORTAbits.RA0 == 1) op_key = 1;
        if(PORTAbits.RA1 == 1) op_key = 2;
        if(PORTAbits.RA2 == 1) op_key = 3;
        if(PORTAbits.RA3 == 1) op_key = 4; 
    }
        __delay_ms(1);
        return op_key;
}

unsigned char calculate(unsigned char x, unsigned char y, unsigned char op){
    unsigned char output = 0;
    if(op == 1)
        output = x + y;
    if(op ==2)
        output = x-y;
    if(op == 3)
        output = x*y;
    if(op == 4)
        output = x/y; 
    return output;
}

void PORT_Output(unsigned char answer){
    unsigned char first_dig = 0;
    unsigned char second_dig = 0;
    unsigned char i = 0;
    unsigned char display1 = 0;
    unsigned char display2 = 0;
    
    if(answer<0)
        PORTBbits.RB7 = 0; 
    first_dig = answer/10;
    second_dig = answer % 10;
    
    if(first_dig == 0) display1 = 0xC0;
    if(first_dig == 1) display1 = 0xF9;
    if(first_dig == 2) display1 = 0xA4;
    if(first_dig == 3) display1 = 0xB0;
    if(first_dig == 4) display1 = 0x99;
    if(first_dig == 5) display1 = 0x92;
    if(first_dig == 6) display1 = 0x82;
    if(first_dig == 7) display1 = 0xF8;
    if(first_dig == 8) display1 = 0x80;
    if(first_dig == 9) display1 = 0x98;
    
    if(second_dig == 0) display2 = 0xC0;
    if(second_dig == 1) display2 = 0xF9;
    if(second_dig == 2) display2 = 0xA4;
    if(second_dig == 3) display2 = 0xB0;
    if(second_dig == 4) display2 = 0x99;
    if(second_dig == 5) display2 = 0x92;
    if(second_dig == 6) display2 = 0x82;
    if(second_dig == 7) display2 = 0xF8;
    if(second_dig == 8) display2 = 0x80;
    if(second_dig == 9) display2 = 0x98;
    
    PORTBbits.RB2 = 1;
    while (PORTAbits.RA3 == 0){
        PORTD = 0;
    }
 
    for(i = 1; i<10000; i++)
    {
        PORTBbits.RB5 = 1;
        PORTD = display1;
        PORTBbits.RB5 = 0;
        PORTBbits.RB6 = 1;
        PORTD = display2; 
        PORTBbits.RB6 = 0;
    }
    PORTD = 0;
    PORTB = 0;
    
}