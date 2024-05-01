
#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include "LCD.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void TF_Luna_Send_Freq(uint8_t header, uint8_t length, uint8_t id, uint8_t freq, uint8_t freq_dec,  uint8_t checksum);
void TF_Luna_Trigger(uint8_t header, uint8_t length, uint8_t id, uint8_t trigger );
uint16_t read_distance();
void LCD_String_xy(char ,char ,const char*);
void LCD_Init();
void MSdelay(unsigned int val);
void LCD_Clear();
void read_bytes_and_display_hex();

int main(void)
{
    SYSTEM_Initialize();
    UART2_Initialize();
    LCD_Init();

    uint8_t header = 0x5A;
    uint8_t freq_ID = 0x03;
    uint8_t freq_decimal = 0x00;
    uint8_t checksum = 0x00;
    uint8_t trigger = 0x04;
    uint8_t freq = 0x00;
    uint16_t distance = 0;
    uint16_t length = 0x06;
    uint8_t payload = 0x00;
    char data[10];
    
    while(1){
    LCD_Clear();
    MSdelay(100);
    LCD_String_xy(1,0,"start");
    MSdelay(500);
   // TF_Luna_Send_Freq(header,length,freq_ID,freq,freq_decimal,checksum);
    //TF_Luna_Trigger(header, trigger, trigger,payload);
    distance = read_distance();
    sprintf(data,"%u",distance);
    strcat(data," CM");	
    LCD_String_xy(2,4,data);
    MSdelay(200);
   
  
        
    //read_bytes_and_display_hex();
    //while(1);
}
}

void TF_Luna_Send_Freq(uint8_t header, uint8_t length, uint8_t id, uint8_t freq, uint8_t freq_dec, uint8_t checksum) {
    uint8_t bytes[6]; // Define an array to hold the data bytes

    // Populate the array with the provided data
    bytes[0] = header;
    bytes[1] = length;
    bytes[2] = id;
    bytes[3] = freq;
    bytes[4] = freq_dec;
    bytes[5] = checksum;

    
    for (uint8_t i = 0; i < 6; i++) {
        if (UART2_IsTxReady()) {
            UART2_Write(bytes[i]); 
        } else {
            LCD_String_xy(1, 0, "TX buffer full"); 
            break; 
        }
    }
}

void TF_Luna_Trigger(uint8_t header, uint8_t length, uint8_t id, uint8_t payload ){
    if(UART2_IsTxReady()){
        UART2_Write(header);
        UART2_Write(length);
        UART2_Write(id);
        UART2_Write(payload);
    }
    else{
        LCD_String_xy(1,0,"TX trigger");
    }
}

uint16_t read_distance(){
    uint8_t distance_L = 0;
    uint8_t distance_H = 0;
    char i = 0;
    uint8_t temp = 0;
    uint8_t header = 0x59;
    uint8_t temp2 = 0;
    
   // for (i=0;i<9;i++){
        if(UART2_IsRxReady()){
            while(1){
            temp = UART2_Read();
            temp2 = UART2_Read();
            if (temp == header && temp2 == header){
            distance_L = UART2_Read();
            if(UART2_IsRxReady())
            distance_H = UART2_Read();
            break;
            }
            }
        }
        
            /*if(i == 2){
            distance_L = UART2_Read();
        }
            else if (i == 3){
                distance_H = UART2_Read();
            }
            else{
                temp = UART2_Read();
            }
    }*/
        else {
            LCD_String_xy(1,0 ,"RX not ready");
            
        }
        
    
uint16_t distance = ((uint16_t)distance_H << 8) | distance_L;
    
    return distance;
}

void read_bytes_and_display_hex() {
    uint8_t received_bytes[9]; // Array to store received bytes
    char hex_string[19]; // String to hold hexadecimal representation (9 bytes * 2 characters per byte + 1 for null terminator)
    
    uint8_t byte_count = 0; // Counter to track number of received bytes

    while (byte_count < 9) {
        // Continuously check for UART receive data
        if (UART2_IsRxReady()) {
            received_bytes[byte_count] = UART2_Read(); // Read byte and store in array
            byte_count++; // Increment byte count

            // Display received bytes in hexadecimal on LCD when all 9 bytes are received
            if (byte_count == 9) {
                // Format received bytes into a hexadecimal string
                hex_string[0] = '\0'; // Ensure string is empty initially
                for (uint8_t i = 0; i < 9; i++) {
                    sprintf(hex_string + strlen(hex_string), "%02X ", received_bytes[i]); // Append byte in hexadecimal format
                }
                LCD_Clear();
                //LCD_String_xy(1, 0, "Received Bytes:");

                // Split the hexadecimal string into two lines on the LCD
                LCD_String_xy(1, 0, hex_string); // Display first part on line 2
                LCD_String_xy(2, 0, hex_string + 16); // Display second part starting from index 16 on line 3
        }
    }
}
}
