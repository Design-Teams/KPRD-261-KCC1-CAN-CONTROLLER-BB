/* 
 * File:   DataProcess.h
 * Author: rahul.chavan
 *
 * Created on June 26, 2024, 11:55 AM
 */

#ifndef DATAPROCESS_H
#define	DATAPROCESS_H


#define OUTPUT_ON  1
#define OUTPUT_OFF 0

#define ADC_Offset 80

#define ADC_Offset_Count 5

#define Firm_Ver 0.04  // version

typedef struct
{
    uint8_t Output_1:1;
    uint8_t Output_2:1;
    uint8_t Output_3:1;
    uint8_t Output_4:1;
    uint8_t Output_5:1;
    uint8_t Output_6:1;
    uint8_t Output_7:1;
    uint8_t Output_8:1;
    uint8_t Output_9:1;
    uint8_t Output_10:1;
    uint8_t Output_11:1;
    uint8_t Output_12:1;
    uint8_t Output_13:1;
    uint8_t Output_14:1;
    uint8_t Output_15:1;
    uint8_t Output_16:1;
}OP_HANDLE;

typedef struct
{
    uint8_t CAN_Buf[9];
    uint32_t CANID;
}CAN_data;

typedef struct
{
    uint8_t Status;
    uint16_t ID;
    uint16_t Act_Load;
    uint16_t Load_Count;
    uint16_t Bat_Vtg;
}NRF_Para;

OP_HANDLE Doutput;
CAN_data CAN_Txpara,CAN_Rxpara;
uCAN_MSG ECAN_TxMSG,ECAN_RxMSG;
NRF_Para NRF;

extern bool Red_Led,serial_diagnost;
//bool diagnost_mode;
uint8_t  DADC[3];//,adc_overflow; CAN_TBuf[9],CAN_RBuf[9],
uint8_t  digital_status,uart1_data_flag,uart2_data_flag,can1_data_flag,nrf_data_flag,NRFP_flag,NRFC_flag;
uint8_t  CAN_RStatus,ADC_PCnt[8],ADC_NCnt[8];//,N_Cap,N_Bat,N_MSB,N_LSB,
uint16_t digital_output;
uint16_t ADC[10],NID,N_Load,N_ADC,temp_ADC[15];//adc_result,
uint32_t N_Serial;//CANRID,CANTID;
volatile uint8_t Uart1_Frame_Flag,Uart2_Frame_Flag,uart1_index,uart2_index;
char Uart1_array[25],Uart2_array[40];
volatile uint32_t can_timeout;
bool Yellow_led;

void Data_Process(void);
void Uart1_Data_Handler(void);
void Uart2_Data_Handler(void);
void Uart1_Data_Send(void);
void EUSART1_Receive_ISR(void);
void EUSART2_Receive_ISR(void);
void Digital_Output_Handler(void);
//void Can_to_Uart_Data(void);
void OSCILLATOR_Initialize(void);
void System_Initialize(void);
void Can_Data_Send(void);
void CAN_Request_Send(void);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* DATAPROCESS_H */
/*
 sprintf(disp,"{,AD,%d,%d,%d,%d,%d,",ADC[0],ADC[1],ADC[2],ADC[3],ADC[4]);
    EUSART1_String(disp);
    sprintf(disp,"%d,%d,%d,%d,%d,",ADC[5],ADC[7],ADC[6],ADC[9],ADC[8]); 
    EUSART1_String(disp);
    sprintf(disp,"%d,%d,%d,}\n",DADC[0],DADC[1],DADC[2]); 
    EUSART1_String(disp);
 */