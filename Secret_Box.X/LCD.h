#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATB                 /* PORTB is used for transmitting data to LCD */

#define LCD_Port TRISB              
#define LCD_Control TRISD

void LCD_Init();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );


/*****************************Main Program*******************************/

//void main(void)
//{       
    //OSCCON=0x72;                   /* Use Internal Oscillator with Frequency 8MHZ */ 
  //  LCD_Init();                    /* Initialize 16x2 LCD */
  //  LCD_String_xy(1,0,"Embedded System!");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
    //LCD_String_xy(2,0,"EE310@SSU");   /*Display string at location(row,location). */
                                   /* This function passes string to display */    
    
//    while(1);           
//}

/****************************Functions********************************/
void LCD_Init()
{
    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* Set PORTB as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* Set PORTD as output PORT LCD Control(RS,EN) Pins */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_Clear()
{
        LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd )
{
    ldata= cmd;            /* Send data to PORT as a command for LCD */   
    RS = 0;                /* Command Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    MSdelay(3); 
}

void LCD_Char(char dat)
{
    ldata= dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    MSdelay(1);
}


void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
        }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}

unsigned char LightScan(char pinNumber){

    unsigned char counter =0; 
    
    if (pinNumber == 1)
    {
    while(counter < 4)
    {
        if(PORTAbits.RA3 == 1)
        {
            MSdelay(500);
            return counter;
        }
                
        if (PORTAbits.RA1 == 1) 
        {
            counter ++;
            PORTCbits.RC7 = 1;
            MSdelay(500);
            PORTCbits.RC7 = 0;
            
        }
    }
    }
    else 
    {
         while(counter < 4)
    {
              if(PORTAbits.RA3 == 1){
                MSdelay(500);  
                return counter;
              }
        if (PORTAbits.RA2 == 1) 
        {
            counter ++;
            PORTCbits.RC7 = 1;
            MSdelay(500);
            PORTCbits.RC7 = 0;
  
        }
    }  
    }
    return counter;
    
}


void Decision(unsigned char input1, unsigned char input2){
    if (input1 == 2 && input2 == 4)
    {
        LCD_String_xy(2,0,"Correct!");
        PORTCbits.RC3 = 1;
        PORTCbits.RC2 = 0;
        MSdelay(500);
        PORTCbits.RC3 = 0;
        return;
        
    }   
    else
    {
        LCD_String_xy(2,0,"Wrong!");
        unsigned int timer = 1000;
   
        while(timer >0)
         {
            MSdelay(1);
            PORTCbits.RC4 = 1;
            MSdelay(1);
            PORTCbits.RC4 = 0;
            timer--;
    }
    }
        return;        
}

void delay_us(unsigned int us) {
    
    for (unsigned int i = 0; i < us; i++) {
        __asm__ volatile ("nop"); //  assembly NOP instruction
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
    }
    return;
}

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
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

