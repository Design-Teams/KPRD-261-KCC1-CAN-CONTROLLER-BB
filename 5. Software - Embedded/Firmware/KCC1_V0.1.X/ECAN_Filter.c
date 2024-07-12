/*=============================================================================
@File name              : CAN_Filter
@Author                 : Rahul Chavan
@Date                   : 03/07/2024
@Software version       : V1.0
@Application Summary    : CAN Initialization & data transfer
@Microcontroller        : PIC18F25K80
@Software IDE           : MPLAB X IDE
@Software IDE Version   : V5.50
@Compiler               : XC8
@Compiler Version       : V2.45
Description             : To Initialize CAN module as per baudrate & transfer/receive data through CAN BUS

============================================================================*/
/*=============================================================================
 * Special Notes if any : This file is used to initialize CAN module as well as transfer/receive data on CAN BUS
 *                  
 =============================================================================*/
/*============================================================================
 * Pin Details:
 * 
 * --------------------------------------------------------------------------
 * | Sr.No. |           Pin Details               |         Purpose         |
 * --------------------------------------------------------------------------
 * |   1    | RC6/CANTX/TX1/CK1/CCP3         (14) | CAN transmitter on PORTC|    |
 * |   2    | RC7/CANRX/RX1/DT1/CCP4         (15) | CAN receiver on PORTC   |
 * |   3    | RB2/CANTX/C1OUT/P1C/CTED1/INT2 (20) | CAN transmitter on PORTB|
 * |   4    | RB3/CANRX/C2OUT/P1D/CTED2/INT3 (21) | CAN receiver on PORTB   |
 * --------------------------------------------------------------------------
 ===============================================================================*/

/*==============================================================================
 * Version History
 * Software Version  : V1.0
 * Date              : 03/07/2024
 ===============================================================================*/

/*==============================================================================
 * List of functions used
 * void InitEcan(uint16_t bitrate,bool filter,uint16_t filter_id);
 * void CanIdFilter(uint8_t Ext,uint32_t CanId);
 * unsigned char ReadCan(uint8_t* Ext,uint32_t* CanId,uint8_t Dlc,uint8_t Rtr, uint8_t* Data);
 * void CanIdWrite(uint8_t Ext,uint32_t CanId);
 * void WriteDlcRtr(uint8_t Dlc,uint8_t Rtr);
 * void WriteData(uint8_t Dlc,uint8_t *Data);
 * void CanWriteMessage(uint8_t Ext,uint32_t CanId,uint8_t Dlc, uint8_t Rtr, uint8_t *Data);
 ===============================================================================*/


#include "ECAN_Filter.h"


/*=============================================================================
* Function Name    : void Init_Ecan(uint16_t bitrate,bool filter,uint16_t filter_id)
* Description      : To Initialize CAN module
* Returned Value   : void
* Parameters       : Can speed, Mask filter, Filter ID
* Example          : Init_Ecan(ECAN_500,true,100);
*
==============================================================================*/
void Init_Ecan(uint16_t bitrate,bool filter,uint16_t filter_id)
{
    CANCON = 0x80;
    while (0x80 != (CANSTAT & 0xE0)); // wait until ECAN is in config mode
    ECANCON = 0x00;
    CIOCON = 0x20;
    if(filter == true)
    {
        RXM0EIDH = 0xFF;
        RXM0EIDL = 0xFF;
        RXM0SIDH = 0xFF;
        RXM0SIDL = 0xE3;
        RXM1EIDH = 0xFF;
        RXM1EIDL = 0xFF;
        RXM1SIDH = 0xFF;
        RXM1SIDL = 0xE3;
          
        RXF0EIDH = 0x00;
        RXF0EIDL = 0x00;
        RXF0SIDH = (uint8_t)((filter_id & 0x0FF8)>>3);
        RXF0SIDL = (uint8_t)((filter_id & 0x0007)<<5);
    }
    else
    {
        RXM0EIDH = 0x00;
        RXM0EIDL = 0x00;
        RXM0SIDH = 0x00;
        RXM0SIDL = 0x00;
        RXM1EIDH = 0x00;
        RXM1EIDL = 0x00;
        RXM1SIDH = 0x00;
        RXM1SIDL = 0x00;
         
        RXF0EIDH = 0x00;
        RXF0EIDL = 0x00;
        RXF0SIDH = 0x00;
        RXF0SIDL = 0x00;
        
    }
    RXF1EIDH = 0x00;
    RXF1EIDL = 0x00;
    RXF1SIDH = 0x00;
    RXF1SIDL = 0x00;
    RXF2EIDH = 0x00;
    RXF2EIDL = 0x00;
    RXF2SIDH = 0x00;
    RXF2SIDL = 0x00;
    RXF3EIDH = 0x00;
    RXF3EIDL = 0x00;
    RXF3SIDH = 0x00;
    RXF3SIDL = 0x00;
    RXF4EIDH = 0x00;
    RXF4EIDL = 0x00;
    RXF4SIDH = 0x00;
    RXF4SIDL = 0x00;
    RXF5EIDH = 0x00;
    RXF5EIDL = 0x00;
    RXF5SIDH = 0x00;
    RXF5SIDL = 0x00;
    if(bitrate == 100)
    {
        BRGCON1 = 0x09; 			
        BRGCON2 = 0xBC; 			
        BRGCON3 = 0x07; 			
    }
    else if(bitrate == 125)
    {
        BRGCON1 = 0x07; 			
        BRGCON2 = 0xBC; 			
        BRGCON3 = 0x07; 			
 	}	
    else if(bitrate == 250)
    {
        BRGCON1 = 0x03; 			
        BRGCON2 = 0xBC; 			
        BRGCON3 = 0x07;
    }
    else if(bitrate == 500)
    {
        BRGCON1 = 0x01; 			
        BRGCON2 = 0xBC; 			
        BRGCON3 = 0x07;
    }
    
    CANCON = 0x00;
    while (0x00 != (CANSTAT & 0xE0)); // wait until ECAN is in Normal mode   
    
}


/*=============================================================================
* Function Name    : Read_ECan.
* Description      : To Read CAN Data with identifier,data length,RTR bit,actual data 
* Parameters       : Five Parameters required to pass
*                   1) uint8_t* Ext.
*                   2) uint32* CanId
*                   3) uint8_t Dlc
*                   4) uint8_t Rtr 
*                   5) uint8_t* Data
* Returned Value   : return status of CAN
*                    unsigned char
* Example          : Read_ECan(0,&CANRID,8,0,RxBuf);
*                    Ext = 0  Standard CAN
*                    CANRID = 201; must be initialize before calling & call by reference
*                    DLC = 7
*                    RTR = 0
*                    RxBuf = pass buffer where data receive
*=============================================================================*/

unsigned char Read_ECan(uint8_t* Ext,uint32_t* CanId,uint8_t Dlc,uint8_t Rtr, uint8_t* Data)
{
	unsigned char rx_msg_flag;
	unsigned char temp_eidh,temp_eidl,temp_sidh,temp_sidl,temp_dlc;
	unsigned char msg_flag=0;
	rx_msg_flag = 0x00;
    //check RXB0CON reg for msg receive
	if(RXB0CONbits.RXFUL)
	{
		//READ IDENTIFIER
		temp_eidh=RXB0EIDH;
		temp_eidl=RXB0EIDL;
		temp_sidh=RXB0SIDH;
		temp_sidl=RXB0SIDL;
        *CanId=0;
		*CanId=((uint32_t)(temp_sidh<<3)+(temp_sidl>>5));
		if((RXB0SIDL&0x08)==0x08)
		{
			*CanId=(*CanId<<2)+(temp_sidl&0x03);
			*CanId=*CanId<<16;
			*CanId=*CanId+((uint32_t)(temp_eidh<<8)+temp_eidl);
    	}
		//READ DATA LENGTH
		temp_dlc=RXB0DLC;
		//READ DATA
		Data[0]=RXB0D0;
		Data[1]=RXB0D1;
		Data[2]=RXB0D2;
		Data[3]=RXB0D3;
		Data[4]=RXB0D4;
		Data[5]=RXB0D5;
		Data[6]=RXB0D6;
		Data[7]=RXB0D7;
		msg_flag=1;
		RXB0CON=0x00;
	}
	else if(RXB1CONbits.RXFUL)
	{
		//READ IDENTIFIER
		temp_eidh=RXB1EIDH;
		temp_eidl=RXB1EIDL;
		temp_sidh=RXB1SIDH;
		temp_sidl=RXB1SIDL;
        *CanId=0;
		*CanId=((uint32_t)(temp_sidh<<3)+(temp_sidl>>5));
		if((RXB0SIDL&0x08)==0x08)
		{
			*CanId=(*CanId<<2)+(temp_sidl&0x03);
			*CanId=*CanId<<16;
			*CanId=*CanId+((uint32_t)(temp_eidh<<8)+temp_eidl);
		}
		//READ DATA LENGTH
		temp_dlc=RXB1DLC;
		//READ DATA
		Data[0]=RXB1D0;
		Data[1]=RXB1D1;
		Data[2]=RXB1D2;
		Data[3]=RXB1D3;
		Data[4]=RXB1D4;
		Data[5]=RXB1D5;
		Data[6]=RXB1D6;
		Data[7]=RXB1D7;
		msg_flag=1;
		RXB1CON=0x00;
    }
	if(msg_flag==1)
	{
		msg_flag=0;
		//CLEAR INTR FLAG
		PIR5bits.RXB1IF=0;
		PIR5bits.RXB0IF=0;
		return(1);
	}
	else
		return(0);    
}
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
void ECanIdWrite(uint8_t Ext,uint32_t CanId)
{
    unsigned int canid;
    canid = (unsigned int)(CanId & 0x0FFFF);
    if (Ext == 1) 
	{
        TXB0EIDH = (uint8_t) (canid & 0xFF);
        TXB0EIDL = (uint8_t) (canid / 256);
        canid    = (unsigned int)( CanId / 0x10000L );
        TXB0SIDH = (uint8_t) (canid & 0x03);
        TXB0SIDL += (uint8_t) ((canid & 0x1C )*8);
    }
    else 
	{
        TXB0EIDH = 0;
        TXB0EIDL = 0;
		TXB0SIDH =(uint8_t)(canid / 8 );
        TXB0SIDL =(uint8_t)((canid & 0x07)*32);//*32
     
    }	

}
/*=============================================================================
* Function Name    : ECAN_WriteDlcRtr.
* Description      : To Write Data length code and RTR bit on the CAN bus 
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Dlc
*                   2) uint8_t Rtr
* Returned Value   : void
* Example          : ECAN_WriteDlcRtr(7,0);
*=============================================================================*/

void ECAN_WriteDlcRtr(uint8_t Dlc,uint8_t Rtr)
{
    if(Rtr==1)
	{
		Dlc = Dlc |0x40;	     //Set the TXRTR bit of TXb0DLC
		TXB0DLC = Dlc & 0x4F;	 //data length register
	}
	else
    TXB0DLC = Dlc & 0x0F;		//data length register
}

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
void ECAN_WriteData(uint8_t Dlc,uint8_t *Data)
{
	unsigned char length,loopCnt;
	unsigned int timeout_cnt = 0xFFFF;

	TXB0CON = 0x00;
    length=Dlc;

	TXB0D0=*Data;Data++;		//data buffer is zero	
	TXB0D1=*Data;Data++;		//data buffer is one
	TXB0D2=*Data;Data++;		//data buffer is two
	TXB0D3=*Data;Data++;		//data buffer is three
	TXB0D4=*Data;Data++;		//data buffer is four
	TXB0D5=*Data;Data++;		//data buffer is five
	TXB0D6=*Data;Data++;		//data buffer is six
	TXB0D7=*Data;				//data buffer is seven
	RCONbits.IPEN    = 1;  
   	TXB0CONbits.TXREQ = 1; //Set the buffer to transmit
	TXB0CONbits.TXPRI0=1;
	TXB0CONbits.TXPRI1=1;
	
	__delay_ms(100);
    while((TXB0CON==0x08)&&(timeout_cnt))
	{
		timeout_cnt--;
	}
}
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
void ECanWriteMessage(uint8_t Ext,uint32_t CanId,uint8_t Dlc, uint8_t Rtr, uint8_t *Data)
{
	ECanIdWrite(Ext,CanId);
	ECAN_WriteDlcRtr(Dlc,Rtr);
	ECAN_WriteData(Dlc,Data);

}


#if 0
/*=============================================================================
* Function Name    : void CanIdFilter
* Description      : To Read CAN Identifier & check wether it is extended or standard
* Parameters       : Two Parameters required to pass
*                   1) uint8_t Ext. 
*                   2) uint32_t CanId
* Returned Value   : void
* Example          : CanIdFilter(0,201);  
*					Ext = 0 means normal CAN 
*					Ext = 1 means extended CAN
*
==============================================================================*/

void CanIdFilter(uint8_t Ext,uint32_t CanId)
{
    unsigned int canid;
    canid = (unsigned int)(CanId & 0x0FFFF);
    if (Ext == 1) 
	{
        RXF0EIDH = (uint8_t) (canid & 0xFF);
        RXF0EIDL = (uint8_t) (canid / 256);
        canid    = (unsigned int)( CanId / 0x10000L );
        RXF0SIDH = (uint8_t) (canid & 0x03);
        RXF0SIDL += (uint8_t) ((canid & 0x1C )*8);
    }
    else 
	{
        RXF0EIDH = 0;
        RXF0EIDL = 0;
		RXF0SIDH =(uint8_t)(canid / 8 );
        RXF0SIDL =(uint8_t)((canid & 0x07)*32);//*32
    }	
   
}

//==================================================================================================================
/*******************************************************************************************************************
* 
* Function Name    : void ReadCanId(uint8 *Ext,uint32 *CanId)
* Returned Value   : void
* Comments         : Read the CAN bus identifier check is extended or standard identifier
*
*******************************************************************************************************************/
//==================================================================================================================
void ReadCanId(uint8_t *Ext,uint32_t *CanId)
{
	unsigned int canId1,canId2,canId3,canId5,canId4;
    *Ext = 0;
    *CanId = 0; canId1=0,canId2=0,canId3=0,canId4=0,canId5=0;
 
    canId1 = RXB0SIDH;				//Receive  Standard ID high Byte
	canId1 = canId1<<3;
	canId2 = RXB0SIDL;				//Receive  Standard ID low Byte
	canId2 = canId2>>5;
    *CanId = canId1 + canId2;

	canId3 = RXB0SIDL;
    if ((canId3 & 0x08)== 0x08) 
     {
        *CanId = (*CanId<<2)+(canId3 & 0x03);
        *CanId <<= 16;
		canId4 = RXB0EIDH;			//Receive  Extended ID high Byte
		canId5 = RXB0EIDL;			//Receive  Extended ID low Byte
        *CanId = *CanId +(canId4<<8)+ canId5;
        *Ext = 1;
     }

 	
}
//==================================================================================================================
/*******************************************************************************************************************
* 
* Function Name    : void ReadDlcRtr(uint8 *Dlc, uint8 *Rtr)
* Returned Value   : void
* Comments         : Read data length of message and Remote tranmission bits. 
*
*******************************************************************************************************************/
//==================================================================================================================
void ReadDlcRtr(uint8_t *Dlc, uint8_t *Rtr)
{
	*Dlc=0;
	*Rtr=0;
    *Dlc = RXB0DLC;		//Receive  length of data in temp_DLC

	if((*Dlc&0x40)==0x40)
	{
		*Rtr=1;			//Receive Remote transmission request
	}
	else
   	*Rtr=0;
}
//==================================================================================================================
/*******************************************************************************************************************
* Function Name    : void ReadData(uint8 *Dlc, uint8 *Data)
* Returned Value   : void
* Comments         : Read data length of message and actual data that received in buffer. 
*
*******************************************************************************************************************/
//==================================================================================================================
void ReadData(uint8_t *Dlc, uint8_t *Data)
{
		unsigned char length;//,loopCnt;
    	length=*Dlc;

		*Data=RXB0D0;Data++;		//Receive data buffer 0 in temp_D0
		*Data=RXB0D1;Data++;		//Receive data buffer 1 in temp_D1
		*Data=RXB0D2;Data++;		//Receive data buffer 2 in temp_D2
		*Data=RXB0D3;Data++;		//Receive data buffer 3 in temp_D3
		*Data=RXB0D4;Data++;		//Receive data buffer 4 in temp_D4
		*Data=RXB0D5;Data++;		//Receive data buffer 5 in temp_D5
		*Data=RXB0D6;Data++;		//Receive data buffer 6 in temp_D6
		*Data=RXB0D7;				//Receive data buffer 7 in temp_D7
}

/*=============================================================================
* Function Name    : Can_Set_BitRate.
* Description      : Set CAN Bus baudrate
* Parameters       : One Parameters required to pass
*                   1) unsigned int Bitrate
* Returned Value   : unsigned char
* Example          : Can_Set_BitRate(ECAN_100);
*=============================================================================*/

unsigned char Can_Set_BitRate(unsigned int Bitrate)//11 MHZ with HS-PLL
{
	switch (Bitrate)
	{
		case ECAN_100:	
				 BRGCON1 = 0x09; 			//0000 0011     //SJW=3TQ       BRP  27
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=8TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  5TQ
				break; 
		case ECAN_125:	
			 BRGCON1 = 0x07; 			//0000 0011     //SJW=3TQ       BRP  15
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=5TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  1TQ
 				 break;	
		case ECAN_250: 						 
				 BRGCON1 = 0x03; 			//0000 0011     //SJW=3TQ       BRP  7
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=5TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  1TQ
				break;
		case ECAN_500: 						
				 BRGCON1 = 0x01; 			//0000 0011     //SJW=3TQ       BRP  3
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=5TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  1TQ
				break;
	    case ECAN_1000:  					
				 BRGCON1 = 0x00; 			//0000 0011     //SJW=3TQ       BRP  1
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=5TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  1TQ
				break;
		default:							
			 	 BRGCON1 = 0x09; 			//0000 0011     //SJW=3TQ       BRP  19
        		 BRGCON2 = 0xBC; 			//1011 1000     //SEG2PHTS 1    sampled once  PS1=5TQ  PropagationT 1TQ  
        		 BRGCON3 = 0x07; 			//0000 0101     //PS2  1TQ
				 break; 
	}
	return (0);
}

#endif
//-------------------------------------------------------------------------------------------------------------------


