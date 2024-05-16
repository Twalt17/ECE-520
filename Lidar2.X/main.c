 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "LCD.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <xc.h>
#include <stdio.h>
#include "mcc_generated_files/system/config_bits.h"
#include "mcc_generated_files/system/clock.h"
#include "mcc_generated_files/system/interrupt.h"
#include "mcc_generated_files/system/pins.h"


void LCD_String_xy(char ,char ,const char*);
void LCD_Init();
void TF_Luna_Send_Freq(uint8_t header, uint8_t length, uint8_t id, uint8_t freq, uint8_t freq_dec, uint8_t checksum);
void TF_Luna_Trigger(uint8_t header, uint8_t length, uint8_t id, uint8_t payload );
void read();
uint8_t received_data;
uint8_t header = 0x5A;
uint8_t freq = 0x0;
uint8_t freq_dec = 0x0;
uint8_t checksum = 0x00;
uint8_t freq_ID = 0x03;
uint8_t trigger = 0x04;

/*
    Main application
*/

int main(void)
{
    PORTE = 0;
    TRISE = 0xFF;
    ANSELE = 0;
    LATE = 0;
    
    SYSTEM_Initialize();
    UART2_Initialize();
    U2CON1bits.ON = 1;
    U2CON0bits.RXEN = 1;
    U2CON0bits.TXEN = 1;
    UART2_ReceiveEnable();
    LCD_Init();
    LCD_String_xy(1,0,"fuck");
    MSdelay(1000);
    
   TF_Luna_Send_Freq(header, 0x6, freq_ID, freq, freq_dec, checksum);

   
   int steps = 5;
   int angle = 0;
   
   while(1){
   if(PORTEbits.RE1 == 1){
       angle = 0;
       printf("%d\n\r",steps);
   MSdelay(3000);
 
   for (int i = 1; i<=steps;i++){  
   printf("%d\n\r", angle);
   MSdelay(2000);
   TF_Luna_Trigger(header, 0x4, trigger, 0x00);
   read();
   MSdelay(2000);
   angle +=15;
}
   }
}
}

   void read(){
    uint8_t Read_header = 0x59;
    uint8_t distance_L = 0;
    uint8_t distance_H = 0;
    uint16_t distance = 0;
    unsigned char count = 0;
    char data[20];
    
    //while(count < 254){
    uint8_t received_bytes[9]; // Array to store received bytes
    char hex_string[37]; // String to hold hex format
    
    uint8_t byte_count = 0; // Counter to track number of received bytes

    while (byte_count < 9){
        // Continuously check for UART receive data
        if (UART1.IsRxReady()) {
            received_bytes[byte_count] = UART1.Read(); // Read byte and store in array
            byte_count++; // Increment byte count

            // Display received bytes in hexadecimal when all 9 bytes are received
        }
    }
        for(uint8_t i = 0; i<6;i++){
            if(received_bytes[i] == Read_header && received_bytes[i+1] == Read_header){
                distance_L = received_bytes[i+2];
                distance_H = received_bytes[i+3];
                distance = ((uint16_t)distance_H << 8) | distance_L;
                sprintf(data, "%u   ", distance);
                LCD_String_xy(2,0,data);
                LCD_String_xy(2,5,"CM");
                
                printf("%u\r\n", distance);
                break;
            }
            
        }
    
    return;
    }
   

    



    
    

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    //INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 


  
void TF_Luna_Send_Freq(uint8_t header, uint8_t length, uint8_t id, uint8_t freq, uint8_t freq_dec, uint8_t checksum) {
    uint8_t bytes[6]; // Define an array to hold the data bytes

    // Populate the array with the provided data
    char bytes_sent = 0;
    bytes[0] = header;
    bytes[1] = length;
    bytes[2] = id;
    bytes[3] = freq;
    bytes[4] = freq_dec;
    bytes[5] = checksum;

    
    while(bytes_sent < 6){
        if (UART1.IsTxReady()) {
            UART1.Write(bytes[bytes_sent]);
            bytes_sent++;
        } 
    }
    }



void TF_Luna_Trigger(uint8_t header, uint8_t length, uint8_t id, uint8_t payload ){
    uint8_t bytes[6];
    char bytes_sent = 0;
    
    bytes[0] = header;
    bytes[1] = length;
    bytes[2] = id;
    bytes[3] = payload;
    
    while(bytes_sent < 4){
        if (UART1.IsTxReady()) {
            UART1.Write(bytes[bytes_sent]);
            bytes_sent++;
        } 
       
        
    }
}
    