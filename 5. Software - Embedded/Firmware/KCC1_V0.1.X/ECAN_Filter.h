/* 
 * File:   ECAN_Filter.h
 * Author: rahul.chavan
 *
 * Created on July 3, 2024, 12:08 PM
 */

#ifndef ECAN_FILTER_H
#define	ECAN_FILTER_H

#include "Library_Header.h"

#define CONFIGMODE			0x80
#define LOOPBACKMODE		0x40
#define LISTENONLYMODE		0x60
#define DIS_SLEEPMODE		0x10
#define NORMALMODE			0x00
#define LEGACYMODE			0x00
#define ENLEGACYMODE		0x40
#define ENFIFOMODE			0x80
#define ECAN_100            0x00                 
#define ECAN_125            0x01                 
#define ECAN_250            0x02                 
#define ECAN_500            0x03                 
#define ECAN_1000           0x04                 
#define TRUE                1
#define FALSE               0

/*=============================================================================
* Function Name    : void Init_Ecan(uint16_t bitrate,bool filter,uint16_t filter_id)
* Description      : To Initialize CAN module
* Returned Value   : void
* Parameters       : Three Parameters required
*                  : uint16_t bitrate = Can speed
*                  : bool filter =  Mask filter
*                  : uint16_t filter_id = Filter ID
* Example          : InitEcan(ECAN_500,true,100);
*
==============================================================================*/
void Init_Ecan(uint16_t bitrate,bool filter,uint16_t filter_id);





/*=============================================================================
* Function Name    : ReadCan.
* Description      : To Read CAN Data with identifier,data length,RTR bit,actual data 
* Parameters       : Five Parameters required to pass
*                   1) uint8_t* Ext.
*                   2) uint32* CanId
*                   3) uint8_t Dlc
*                   4) uint8_t Rtr 
*                   5) uint8_t* Data
* Returned Value   : return status of CAN
*                    unsigned char
* Example          : ReadCan(0,&CANRID,7,0,RxBuf);
*                    Ext = 0  Standard CAN
*                    CANRID = 201; must be initialize before calling & call by reference
*                    DLC = 7
*                    RTR = 0
*                    RxBuf = pass buffer where data receive
*=============================================================================*/

unsigned char Read_ECan(uint8_t* Ext,uint32_t* CanId,uint8_t Dlc,uint8_t Rtr, uint8_t* Data);

/*=============================================================================
* Function Name    : ECanIdWrite.
* Description      : To Write Extended or Standard CAN ID on the CAN bus 
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Ext
*                   2) uint32_t CanId
* Returned Value   : void
* Example          : ECanIdWrite(0,201);
*                    Ext = 0 Standard CAN
*                    CanId example 201
*=============================================================================*/

void ECanIdWrite(uint8_t Ext,uint32_t CanId);


/*=============================================================================
* Function Name    : ECAN_WriteDlcRtr.
* Description      : To Write Data length code and RTR bit on the CAN bus 
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Dlc
*                   2) uint8_t Rtr
* Returned Value   : void
* Example          : ECAN_WriteDlcRtr(7,0);
*=============================================================================*/

void ECAN_WriteDlcRtr(uint8_t Dlc,uint8_t Rtr);


/*=============================================================================
* Function Name    : ECAN_WriteData.
* Description      : To Write Data length and actual data to be send on the can bus  
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Dlc
*                   2) uint8_t *Data
* Returned Value   : void
* Example          : ECAN_WriteData(7,*Data);
*                    *Data pass pointer of data
*=============================================================================*/

void ECAN_WriteData(uint8_t Dlc,uint8_t *Data);


/*=============================================================================
* Function Name    : ECanWriteMessage.
* Description      : CAN write message on bus write ID extended or Standard,data length code,rtr and actual data  
* Parameters       : Five Parameters required to pass
*                   1) uint8_t Ext
*                   2) uint32_t CanId
 *                  3) uint8_t Dlc
 *                  4) uint8_t Rtr
 *                  5) uint8_t *Data
* Returned Value   : void
* Example          : ECanWriteMessage(0,201,7,0,data_buffer);
*=============================================================================*/

void ECanWriteMessage(uint8_t Ext,uint32_t CanId,uint8_t Dlc, uint8_t Rtr, uint8_t *Data);
//==========================================================================================================================

#if 0
==========================================================================================================================
void ReadCanId(uint8_t *Ext,uint32_t *CanId);
==========================================================================================================================
void ReadDlcRtr(uint8_t *Dlc, uint8_t *Rtr);
==========================================================================================================================
void ReadData(uint8_t *Dlc, uint8_t *Data);
==========================================================================================================================
/*=============================================================================
* Function Name    : void CanIdFilter
* Description      : To Read CAN Identifier & check wether it is extended or standard
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Ext. 
*                   2) uint32_t CanId
* Returned Value   : void
*  Example          : CanIdFilter(0,201);  
*					Ext = 0 means normal CAN 
*					Ext = 1 means extended CAN
==============================================================================*/
void CanIdFilter(uint8_t Ext,uint32_t CanId);


/*==========================================================================
* Function Name    : Can_Set_BitRate.
* Description      : Set CAN Bus baudrate
* Parameters       : One Parameters required to pass
*                   1) unsigned int Bitrate
* Returned Value   : unsigned char
* Example          : Can_Set_BitRate(ECAN_100);
============================================================================*/

unsigned char Can_Set_BitRate(unsigned int Bitrate);

#endif
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ECAN_FILTER_H */

