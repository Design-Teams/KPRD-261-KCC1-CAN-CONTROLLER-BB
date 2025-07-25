/* 
 * File:   main.h
 * Author: rahul.chavan
 *
 * Created on June 26, 2024, 11:50 AM
 */

#ifndef MAIN_H
#define	MAIN_H


#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>
#include "Timer0.h"
#include "EUSART.h"
#include "ecan.h"
//#include "ECAN_Filter.h"
#include "SPI.h"
#include "InternalADC.h"
#include "MC33972.h"
#include "MC33996.h"
#include "Library_Header.h"
#include "DataProcess.h"
#include "Internal_EEPROM.h"
//#include "MP3.h"
#include "CRC.h"
#include "Moving_Filter.h"
#include "GPIO.h"


#define OUTPUT_ON  1
#define OUTPUT_OFF 0

#define G_LED   LATCbits.LATC0
#define Y_LED   LATAbits.LATA1 
#define R_LED   LATAbits.LATA2 

volatile uint16_t ms_count,can_count,Led_Count,blink_flag,Watchdog_count;
extern volatile uint32_t can_timeout;
bool Red_Led,serial_diagnost;
extern bool Yellow_led;
void Delay_Ms(uint16_t delay);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

