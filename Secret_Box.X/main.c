/*
 * File:   main.c
 * Author: Tyler
 *
 * Created on April 20, 2024, 1:31 PM
 */

#include "Config.h"
#include <xc.h>
#include "LCD.h"
#include <stdlib.h>
#include "Initialization.h"
void initialize();
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void);
void LCD_Init();
void LCD_String(const char *msg);
void LCD_String_xy(char row,char pos,const char *msg);
void LCD_Char(char x);
void LCD_Clear();
unsigned char LightScan(char pinNumber);
void Decision(unsigned char input1, unsigned char input2);
void delay_us(unsigned int us);
unsigned char input = 0;
unsigned char input2 = 0;



void main(void) {
    
    initialize();
    while(1)
    {
    LCD_Init();                    /* Initialize 16x2 LCD */
    LCD_Clear();
    input = LightScan(1);
    LCD_Char(input + '0');
    input2 = LightScan(2);
    LCD_Char(input2 + '0');
    Decision(input,input2);
    //__delay_ms(10000);                              /* This function passes string to display */    
    } 
    return;
}
