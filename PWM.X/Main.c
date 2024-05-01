/*
 * File:   Main.c
 * Author: Tyler
 *
 * Created on April 28, 2024, 10:25 AM
 */


#include <xc.h> // must have this
#include "PWM.h" // must have this
#include "config.h" // must have this -  XC8_ConfigFile.h




#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec

#define myLED  PORTAbits.RA1
uint16_t PWM2_INITIALIZE_DUTY_VALUE = 0;
void MSdelay(unsigned int val);


uint16_t checkdutyCycle;
char preScale;
_Bool pwmStatus;

void main (void) {
    OSCSTATbits.HFOR =1; // enable  HFINTOSC Oscillator (see clock schematic))
    OSCFRQ=0x02; // 00=1 MHZ, 02=4MHZ internal - see page 106 of data sheet
    
    ANSELA = 0b00000000;    
    TRISAbits.TRISA2= 0;//ra0 = output
    PORTAbits.RA2= 0;//sets ra0 to 0 to start
    TRISAbits.TRISA3 = 1; // ra3 = input
    
    TMR2_Initialize();
    TMR2_StartTimer();        
    
    while(1){
    PWM2_INITIALIZE_DUTY_VALUE += 100;
    
    if (PWM2_INITIALIZE_DUTY_VALUE > 1000)
        PWM2_INITIALIZE_DUTY_VALUE = 100;
    
    PWM_Output_D8_Enable();
    PWM2_Initialize();
    PWM2_LoadDutyValue(PWM2_INITIALIZE_DUTY_VALUE ); // initialize CCPR2H/L
   // PWM_Output_D8_Disable();
   // TMR2_StopTimer();  

    // Duty Cycle in percentage 
    checkdutyCycle =(uint16_t)((100UL*PWM2_INITIALIZE_DUTY_VALUE)/(4*(T2PR+1)));
    // binary value of Register T2CON.PRESCALE
    preScale = ((T2CON >> 4) & (0x0F)); 
    MSdelay(500);
    while (PORTAbits.RA3 == 0) {
        pwmStatus = PWM2_OutputStatusGet();
        PORTAbits.RA2 = pwmStatus;
        //T2CON=0x00; // stop the timer & do what you have to do
        if (PIR4bits.TMR2IF == 1) {
            PIR4bits.TMR2IF = 0;
            myLED ^= 1; // ISR code goes here
        }
    }
    }
}