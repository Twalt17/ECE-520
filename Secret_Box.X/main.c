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

/*void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
    for(char i =0; i<3;i++)
    {
    unsigned int timer = 100;
    
    while(timer >0)
    {
    delay_us(100);
    PORTCbits.RC4 = 1;
    delay_us(100);
    PORTCbits.RC4 = 0;
    timer--;
    }
    
    timer = 200;
    
    while(timer >0)
    {
    delay_us(50);
    PORTCbits.RC4 = 1;
    delay_us(50);
    PORTCbits.RC4 = 0;
    timer--;
    }
    
    timer = 400;
    
    while(timer >0)
    {
    delay_us(25);
    PORTCbits.RC4 = 1;
    delay_us(25);
    PORTCbits.RC4 = 0;
    timer--;
    }
    }
    
    PORTCbits.RC4 = 1;
    PIR1bits.INT0IF = 0;  // always clear the interrupt flag when done
    return;
    }
 */


void main(void) {
     //OSCCON=0x72;                   /* Use Internal Oscillator with Frequency 8MHZ */ 
   /* TRISA = 0xFF;   // make PORTA input
    ANSELA = 0; //digital
    PORTA = 0;
    TRISC = 0x00;
    ANSELC = 0;
    PORTC = 0;
    PORTCbits.RC4 = 1;
    INTCON0bits.INT0EDG = 1;
    INT0PPS = PORTAbits.RA0; 
    WPUA=0xFF;
    INTCON0bits.IPEN = 1; // Enable interrupt priority
    INTCON0bits.GIEH = 1; // Enable high priority interrupts
    INTCON0bits.GIEL = 1; // Enable low priority interrupts
    INTCON0bits.INT0EDG = 1; // Interrupt on rising edge of INT0 pin
    IPR1bits.INT0IP = 1; // high priority
    PIE1bits.INT0IE = 1;

    PIR1bits.INT0IF = 0;  //Clear interrupt flag
  */
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
