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
uint8_t received_data;
/*
    Main application
*/

int main(void)
{
    char data[20];
    SYSTEM_Initialize();
    UART2_Initialize();
    U2CON1bits.ON = 1;
    U2CON0bits.RXEN = 1;
    UART2_ReceiveEnable();
    LCD_Init();
    LCD_String_xy(1,0,"programmed");
    MSdelay(1000);
    uint8_t header = 0x59;
    uint8_t distance_L = 0;
    uint8_t distance_H = 0;
    bool header_received = false;
    uint16_t distance = 0;
   
     
   LCD_Clear();
   while(1){
    
    uint8_t received_bytes[9]; // Array to store received bytes
    char hex_string[37]; // String to hold hexadecimal representation (9 bytes * 2 characters per byte + 8 spaces + 1 for null terminator)
    
    uint8_t byte_count = 0; // Counter to track number of received bytes

    while (byte_count < 9){
        // Continuously check for UART receive data
        if (UART2.IsRxReady()) {
            received_bytes[byte_count] = UART2.Read(); // Read byte and store in array
            byte_count++; // Increment byte count

            // Display received bytes in hexadecimal when all 9 bytes are received
        }
    }
    if (byte_count == 9){
        for(uint8_t i = 0; i<6;i++){
            if(received_bytes[i] == header && received_bytes[i+1] == header){
                distance_L = received_bytes[i+2];
                distance_H = received_bytes[i+3];
                distance = ((uint16_t)distance_H << 8) | distance_L;
                sprintf(data, "%u  ", distance);
                LCD_String_xy(2,0,data);
                LCD_String_xy(2,5,"CM");
                break;
            }
            
        }
    }
        
           /* if (byte_count == 9) {
                // Format received bytes into a hexadecimal string
                hex_string[0] = '\0'; // Ensure string is empty initially
                for (uint8_t i = 0; i < 9; i++) {
                    sprintf(hex_string + strlen(hex_string), "%02X ", received_bytes[i]); // Append byte in hexadecimal format
                }
                
                
                // Display hexadecimal string on LCD
                
                

                // Split the hexadecimal string into two lines on the LCD
                LCD_String_xy(1, 0, hex_string+12; // Display first part on line 2
               // LCD_String_xy(2, 0, hex_string + 16); 
            }*/
        }
    }
   

    



    
    

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    //INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 


  

    