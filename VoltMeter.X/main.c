/*
 * File:   main.c
 * Author: Tyler
 *
 * Created on April 27, 2024, 11:16 AM
 */


#include <xc.h>
#include "Functions.h"
#include "Config.h"
#include "Initialization.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void LCD_Init();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );
void initialize();
void ADC_Init(void);

#define Vref 5.0 // voltage reference 
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];

/*****************************Main Program*******************************/

void main(void)
{       
    initialize();
    ADC_Init();        
    LCD_Init();     
    LCD_String_xy(1,0,"The Input Voltage:");    
 
        while(1){                     
        ADCON0bits.GO = 1; 
        while (ADCON0bits.GO); 
        digital = (ADRESH*256) | (ADRESL);
        voltage= digital*((float)Vref/(float)(4096));
        sprintf(data,"%.2f",voltage);
        strcat(data," V");	
        LCD_String_xy(2,4,data);
        ADCON0bits.GO = 0;
        }
               
}

