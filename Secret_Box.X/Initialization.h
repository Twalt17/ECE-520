void initialize(){

    TRISA = 0xFF;   // make PORTA input
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
}