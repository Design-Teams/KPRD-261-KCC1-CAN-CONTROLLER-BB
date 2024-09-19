/*=============================================================================
@File name              : KCC1_V1.0
@Author                 : Rahul Chavan
@Date                   : 26/06/2024
@Software version       : V1.0
@Application Summary    : GPIO interface & communicate on CAN Bus
@Microcontroller        : PIC18F25K80
@Software IDE           : MPLAB X IDE
@Software IDE Version   : V5.50
@Compiler               : XC8
@Compiler Version       : V2.45
Description             : To read Analog & digital inputs,control digital outputs & Audio module
============================================================================*/
/*============================================================================
 * Version 1.0.0 is used as a major milestone, indicating that the software is "complete", 
 * that it has all major features, and is considered reliable enough for general release.
 * Any minimal change in code , increment right side number after decimal point.	(Bug Fix) : //if needed
 * Any minor change in code , increment right side number after decimal point.   	(recommended)	?????
 * Any major change in code , increment Left side number before decimal point.
*/

/*============================================================================
 * Pin Details:
 * 
 * ---------------------------------------------------------------------
 * | Sr.No. |            Pin Details              |      Purpose        |
 * ---------------------------------------------------------------------
 * |   1    | RA3/VREF+/AN3                  (2)  | Analog input        |
 * |   2    | RC0/SOSCO/SCLKI                (8)  | Debug LED           |
 * |   3    | RC1/SOSCI                      (9)  | MC33996 Reset       |
 * |   4    | RC2/T1G/CCP2                   (10) | MC33996 Chip Select |
 * |   5    | RC3/REFO/SCL/SCK               (11) | SPI peripheral clock|
 * |   6    | RC4/SDA/SDI                    (12) | Serial Data in      |
 * |   7    | RC5/SDO                        (13) | Serial Data Out     |
 * |   8    | RC6/CANTX/TX1/CK1/CCP3         (14) | EUSART1 transmitter |
 * |   9    | RC7/CANRX/RX1/DT1/CCP4         (15) | EUSART1 receiver    |
 * |  10    | RB2/CANTX/C1OUT/P1C/CTED1/INT2 (20) | CAN Transmit        |
 * |  11    | RB3/CANRX/C2OUT/P1D/CTED2/INT3 (21) | CAN Receive         |
 * |  12    | RB5/T0CKI/T3CKI/CCP5/KBI1      (23) | Software UART Tx    |
 * |  13    | RB6/PGC/TX2/CK2/KBI2           (24) | EUSART2 transmitter |
 * |  14    | RB7/PGD/T3G/RX2/DT2/KBI3       (25) | EUSART2 receiver    |
 * |  15    | RA0/CVREF/AN0/ULPWU            (27) | MC33972 Chip Select |
 * ----------------------------------------------------------------------
 ===============================================================================*/
/*==============================================================================
 * Version History
 * Software Version  : V1.0
 * Date              : 26/06/2024
 ===============================================================================*/

#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG//HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = HS1       // Oscillator (HS oscillator (Medium power, 4 MHz - 16 MHz))
#pragma config PLLCFG = ON      // PLL x4 Enable bit (Enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 2         // Brown-out Reset Voltage bits (2.0V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF//NOSLP      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 512      // Watchdog Postscaler (1:512)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-01FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 02000-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-01FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 02000-03FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

#include"main.h"

#define OUTPUT_ON  1
#define OUTPUT_OFF 0

#define G_LED   LATCbits.LATC0
#define Y_LED   LATAbits.LATA1 
#define R_LED   LATAbits.LATA2 


/*=============================================================================
 * Function     : Delay_Ms.
 * Description  : Used for delay 
 * Parameters   : Pass one parameter only
 *              : uint16_t delay :- send 0-65535 value only 
 * Return       : Void.
 * Example      : Delay_Ms(1); It can generate 1 millisecond delay
===========================================================================================================================*/

void Delay_Ms(uint16_t delay)
{
    ms_count = delay;
    while(ms_count>0);
}


void main(void) 
{
    uint8_t can_frame = 0;
    CLRWDT();
    System_Initialize();
    CLRWDT();
    blink_flag = true;
    Led_Count = 500;
    can_timeout = 10000;
    Watchdog_count = 800;
    //serial_diagnost = true;
    while(1)
    {
        Data_Process();
        if(can_count == 0)
        {
            if(serial_diagnost == true)
            {
                Uart1_Data_Send();
            }
            //Uart1_Data_Send();
            if(can_frame == 0)
            {
                Can_Digital_Data_Send();
                can_frame++;
                can_count = 200;
            }
            else
            {
                Can_Analog_Data_Send();
                can_frame = 0;
                can_count = 400;
            }
            
        }
        if(Uart2_Frame_Flag == 1)
        {
            Uart2_Data_Handler();
            Uart2_Frame_Flag = 0;
        }
        if(Watchdog_count == 0)
        {
            Watchdog_count = 800;
            CLRWDT();
        }
    }
     return;
}

/*=============================================================================
 * Function     : __interrupt() INTERRUPT_InterruptManager.
 * Description  : when interrupt occur then this function call automatically 
 * Parameters   : Void 
 * Return       : Void.
===========================================================================================================================*/

void __interrupt() INTERRUPT_InterruptManager (void)
{
    /* Check if TMR0 interrupt is enabled and if the interrupt flag is set */
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
        if(Watchdog_count>0)
        {
            Watchdog_count--;
        }
        if(ms_count>0)
        {
            ms_count--;
        }
        if(can_count>0)
        {
            can_count--;
        }
        if(Led_Count>0)
        {
            Led_Count--;
        }
        if(can_timeout>0)
        {
            can_timeout--;
        }
        if((blink_flag == true) &&(Led_Count == 0))
        {
            if(Red_Led == true)
            {
                G_LED = 0;
                R_LED =~R_LED; 
            }
            else
            {
                R_LED = 0;
                G_LED =~ G_LED;
            }
            if(Yellow_led == true)
            {
               Y_LED =~ Y_LED; 
            }
            else
            {
                Y_LED = 0;
            }
            Led_Count = 500;
        }
    }
    else if(INTCONbits.PEIE == 1)
    {
        if(PIE1bits.RC1IE == 1 && PIR1bits.RC1IF == 1)
        {
          EUSART1_Receive_ISR();
          PIR1bits.RC1IF = 0;
        } 
        if(PIE3bits.RC2IE == 1 && PIR3bits.RC2IF == 1)
        {
           EUSART2_Receive_ISR();
           PIR3bits.RC2IF = 0;
        } 
        if(PIE5bits.RXB1IE == 1 && PIR5bits.RXB1IF == 1)
        {
            ECAN_RXB1I_ISR();
        } 
        if(PIE5bits.RXB0IE == 1 && PIR5bits.RXB0IF == 1)
        {
            ECAN_RXB0I_ISR();
        } 
    }
}