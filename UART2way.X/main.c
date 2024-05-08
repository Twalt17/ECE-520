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
#include <string.h>

#define MAX_COMMAND_LEN 8
uint8_t command[MAX_COMMAND_LEN];
uint8_t index = 0;
uint8_t read_msg;
void UART_ProcessCommand(void);
void UART_executeCommand(char *command);
/*
    Main application
*/

int main(void)
{
    ANSELB = 0;
    TRISB = 0;
    PORTB = 0;
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable(); 
    UART2_Initialize();
    UART2.RxCompleteCallbackRegister(&UART_ProcessCommand);
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
   
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    printf("In the terminal, send 'A' to turn the LED on, and 'B' to turn it off.\r\n");
    printf("Note: commands 'A' and 'B' are case sensitive.\r\n");
    while(1)
    {
    }    
}


void UART_executeCommand(char *command)
{
    if(strcmp(command, "A") == 0)
    {
        LED_SetHigh();
        printf("OK, LED ON.\r\n");
    }
    else if (strcmp(command, "B") == 0)
    {
        LED_SetLow();
        printf("OK, LED OFF.\r\n");
    }
    else
    {
        printf("Incorrect command.\r\n");
    }
}
void UART_ProcessCommand(void)
{
    
    if(UART2.IsRxReady())
    {
        read_msg = UART2.Read();
        if(read_msg != '\n' && read_msg != '\r')
        {
            command[index++] = read_msg;
            if((index) > MAX_COMMAND_LEN)
            {
                (index) = 0;
            }
        }
        if(read_msg == '\n')
        {
            command[index] = '\0';
            index = 0;
            UART_executeCommand(command);
        }
    }
}
