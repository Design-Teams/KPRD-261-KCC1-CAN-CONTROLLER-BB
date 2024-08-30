#include"main.h"

#define MSDI_CS   LATAbits.LATA0  // MSDI CS Pin LATCbits.LATC2
#define MC_RST    LATCbits.LATC1  // MC33996 output reset LATBbits.LATB1
#define MC_CS_PIN LATCbits.LATC2  // MC33996 output LATBbits.LATB0

//#define SUart_Tx  LATBbits.LATB5
#define RS485_Ctrl  LATBbits.LATB5

#define Wire_Break_min 1790
#define Wire_Break_max 1995
#define Switch_Open_Min 1000
#define Switch_Open_Max 1700
#define Switch_Close_Ground 100
#define Switch_Close_Battery 3000



inline void MC33972_CS(bool stat)
{
    MSDI_CS = stat;
}
inline void MC33996_CS(bool stat)
{
    MC_CS_PIN = stat;
}

inline void MC33996_RST(bool stat)
{
    MC_RST = stat;
}

inline void Soft_Uart_Tx(bool stat)
{
    RS485_Ctrl = stat;
}

/*=============================================================================
 * Function     : OSCILLATOR_Initialize.
 * Description  : Used for Oscillator initialize 
 * Parameters   : void 
 * Return       : Void.
 * Example      : OSCILLATOR_Initialize();
===========================================================================================================================*/

void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; HFIOFS not stable; IDLEN disabled; IRCF 8MHz_HF; OSCCON
    OSCCON = 0x60;
    // SOSCGO disabled; MFIOSEL disabled; SOSCDRV Low Power; 
    OSCCON2 = 0x00;
    // INTSRC INTRC; PLLEN disabled; TUN 0; 
    OSCTUNE = 0x00;
    // ROSEL System Clock(FOSC); ROON disabled; ROSSLP Disabled in Sleep mode; RODIV Fosc; 
    REFOCON = 0x00;
}

/*=============================================================================
 * Function     : System_Initialize.
 * Description  : Used for GPIO,Low level module initialize 
 * Parameters   : void 
 * Return       : Void.
 * Example      : System_Initialize();
===========================================================================================================================*/

void System_Initialize(void)
{
    uint8_t eep_read1=0,eep_read2=0,eep_read3=0;
    char buf[20]={0};
    uint16_t CAN_Speed=0;
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    //CS_TRIS = 0;
    TRISCbits.TRISC2 = 0;
    //CS_PIN = 1;
    LATCbits.LATC2 = 1;
    CM1CON = 0x00;
    TRISCbits.TRISC0 = 0;
    LATBbits.LATB2 = 1;
     CLRWDT();
    OSCILLATOR_Initialize();            // Oscillator initialize
    TMR0_Initialize(1);                 // Timer0 initialize with 1 ms
    EUSART1_Initialize(48000000,57600); // EUSART1 Initialize
    EUSART2_Initialize(48000000,9600);  // EUSART2 Initialize
    //Soft_Uart_Initialize();             // Soft Uart Initialize
    LATBbits.LATB2 = 1;
    CLRWDT();
    //Delay_Ms(200);
    
    
    //EEPROM memory mapping
    // CANID store at eeprom 1,2,3 location
    // CANSPEED store at eeprom 4,5,6 location
    // Factory reset stored at eeprom 7,8,9 location
    
    eep_read1 = Eeprom_Read(7); eep_read2 = Eeprom_Read(8); eep_read3 = Eeprom_Read(9);
    
    //read = Eeprom_Read(5);  // Read eeprom
   // if(read != 'R')        // if valid data not found then set default parameter
    if(((eep_read1 != 'R')&&(eep_read2 != 'R'))||((eep_read2 != 'R')&&(eep_read3 != 'R'))||((eep_read1 != 'R')&&(eep_read3 != 'R')))
    {
      CAN_Txpara.CANID = 409;
      Eeprom_Write(1,'1');Eeprom_Write(2,'1');Eeprom_Write(3,'1');
      CLRWDT();
      eep_read1 = Eeprom_Read(1); eep_read2 = Eeprom_Read(2); eep_read3 = Eeprom_Read(3);
      if(((eep_read1 != '1')&&(eep_read2 != '1'))||((eep_read2 != '1')&&(eep_read3 != '1'))||((eep_read1 != '1')&&(eep_read3 != '1')))
      {
         Eeprom_Write(1,'1');Eeprom_Write(2,'1');Eeprom_Write(3,'1');
         EUSART1_String("CANID stored at eeprom failed\r\n");
      }
      else
      {
          EUSART1_String("CANID stored at eeprom Success\r\n");
      }
          
      CAN_Speed = 250;
      Eeprom_Write(4,'3');Eeprom_Write(5,'3');Eeprom_Write(6,'3');
      CLRWDT();
      eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5); eep_read3 = Eeprom_Read(6);
      if(((eep_read1 != '3')&&(eep_read2 != '3'))||((eep_read2 != '3')&&(eep_read3 != '3'))||((eep_read1 != '3')&&(eep_read3 != '3')))
      {
         Eeprom_Write(4,'3');Eeprom_Write(5,'3');Eeprom_Write(6,'3');
         EUSART1_String("CANspeed stored at eeprom failed\r\n");
      }
      else
      {
          EUSART1_String("CANspeed stored at eeprom Success\r\n");
      }
      
      //Eeprom_Write(1,3);
      //Eeprom_Write(5,'R');
      Eeprom_Write(7,'R');Eeprom_Write(8,'R');Eeprom_Write(9,'R');
      CLRWDT();
      eep_read1 = Eeprom_Read(7); eep_read2 = Eeprom_Read(8); eep_read3 = Eeprom_Read(9);
      if(((eep_read1 != 'R')&&(eep_read2 != 'R'))||((eep_read2 != 'R')&&(eep_read3 != 'R'))||((eep_read1 != 'R')&&(eep_read3 != 'R')))
      {
         Eeprom_Write(7,'R');Eeprom_Write(8,'R');Eeprom_Write(9,'R');
         EUSART1_String("Factory setting stored at eeprom failed\r\n");
      }
      else
      {
          EUSART1_String("Factory setting stored at eeprom Success\r\n");
      }
    }
    else
    {
       eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5);eep_read3 = Eeprom_Read(6);    // Read CAN Speed setting
       if(((eep_read1 == '1')&&(eep_read2 == '1'))||((eep_read2 == '1')&&(eep_read3 == '1'))||((eep_read3 == '1')&&(eep_read1 == '1')))//if(read == 1)
       {
           CAN_Speed = 100;
       }
       else if(((eep_read1 == '2')&&(eep_read2 == '2'))||((eep_read2 == '2')&&(eep_read3 == '2'))||((eep_read3 == '2')&&(eep_read1 == '2')))//if(read == 2)
       {
           CAN_Speed = 125;
       }
       else if(((eep_read1 == '3')&&(eep_read2 == '3'))||((eep_read2 == '3')&&(eep_read3 == '3'))||((eep_read3 == '3')&&(eep_read1 == '3')))//if(read == 3)
       {
           CAN_Speed = 250;
       }
       else if(((eep_read1 == '4')&&(eep_read2 == '4'))||((eep_read2 == '4')&&(eep_read3 == '4'))||((eep_read3 == '4')&&(eep_read1 == '4')))//if(read == 4)
       {
           CAN_Speed = 500;
       }
       //read = Eeprom_Read(2);    // Read CAN ID MAIN/AUX
       eep_read1 = Eeprom_Read(1); eep_read2 = Eeprom_Read(2);eep_read3 = Eeprom_Read(3);    // Read CAN Speed setting
       if(((eep_read1 == '1')&&(eep_read2 == '1'))||((eep_read2 == '1')&&(eep_read3 == '1'))||((eep_read3 == '1')&&(eep_read1 == '1')))//if(read == 1)if(read == 1)
       {
          CAN_Txpara.CANID = 409; 
       }
       else if(((eep_read1 == '2')&&(eep_read2 == '2'))||((eep_read2 == '2')&&(eep_read3 == '2'))||((eep_read3 == '2')&&(eep_read1 == '2')))//if(read == 2)else if(read == 2)
       {
          CAN_Txpara.CANID = 410; 
       }
       
    }
    if((CAN_Txpara.CANID == 409)&&(CAN_Speed == 250))
    {
         EUSART1_String("Default Setting\r\n");
    }
    else
    {
        EUSART1_String("User Setting\r\n"); 
    }
     CLRWDT(); 
    //Init_Ecan(CAN_Speed,true,(uint16_t)CAN_Txpara.CANID);
    ECAN_Initialize(CAN_Speed,true,CAN_Txpara.CANID);
    INTCONbits.PEIE = 1;               // Peripheral Interrupt Enable
    INTCONbits.GIE  = 1;               // Global Interrupt Enable
    
    SPI_Init_Master(SPI_MODE1);        // SPI Initialize with mode 1
    ADC_Initialize();                  // Internal ADC Initialize
    ANCON0 = 0x08;                     // Select ADC pin 
    ANCON1 = 0x00;
    TRISAbits.TRISA3 = 1;
    TMR0_StartTimer();                 // Start Timer0
    MC_RST = 1;
    CLRWDT();
    Delay_Ms(500);
    CLRWDT();
    Delay_Ms(500);
    CLRWDT();
    Delay_Ms(500);
    CLRWDT();
    Delay_Ms(500);
   
    if(MC33996_SPI_Check() == 1)        // MC33996 output ic SPI integrity check 
    {
      EUSART1_String("SPI Check OK\r\n"); 
      LATCbits.LATC0 = 1; // G LED Pin
      Delay_Ms(300);
      CLRWDT();
      LATCbits.LATC0 = 0; // G LED Pin
    }
    else
    {
        EUSART1_String("SPI Check error\r\n"); 
        LATAbits.LATA2 = 1; // R LED Pin
        Delay_Ms(500);
        CLRWDT();
        Delay_Ms(500);
        CLRWDT();
        LATAbits.LATA2 = 1; // R LED Pin
    }
    
    MC33996_Init();                         //MC33996 Initialization
    Delay_Ms(100);
    CLRWDT();
    MC33972_Init();                         //MC33972 Initialization
    EUSART1_String("MSDI init done\r\n"); 
    Delay_Ms(500);
    CLRWDT();
    LATBbits.LATB3   = 0;
	TRISBbits.TRISB2 = 0; // CAN Tx
	TRISBbits.TRISB3 = 1;
    sprintf(buf,"KCC1_V%0.2f CANID = %d,CANSpeed = %d\r\n",Firm_Ver,(uint16_t)CAN_Txpara.CANID,CAN_Speed);
    EUSART1_String(buf);
    
    
    LATAbits.LATA1 = 1; // LED Pin
    Delay_Ms(300);
    CLRWDT();
    LATAbits.LATA1 = 0;
     Delay_Ms(30);
    LATAbits.LATA2 = 1; // LED Pin
    Delay_Ms(300);
    LATAbits.LATA2 = 0;
    CAN_Request_Send();
    CLRWDT();
}
/*=============================================================================
 * Function     : Uart1_Data_Handler.
 * Description  : UART1 received data decoding
 * Parameters   : void 
 * Return       : Void.
 * Example      : Uart1_Data_Handler();
===========================================================================================================================*/

void Uart1_Data_Handler(void)
{
    char* token,token_buf[9][10];//,lbuf[5];
    uint8_t k;
    if(memcmp(((char*)Uart1_array+1),",OD,",4) == 0)          //Output data {,OD,digital_output,mp3_stat,track_num,}
    {
        uart1_data_flag = true;
        //EUSART1_String("In OD loop\n");
        token = strtok((char*)Uart1_array, ",");
        token = strtok(NULL,",");
        for(k=0; k<1; k++)
        {
            token = strtok(NULL,",");
            sprintf(token_buf[k],"%s",token);
        }
        digital_output  = (uint16_t)(atol(token_buf[0]));
        
    }
    else if(memcmp(((char*)Uart1_array+1),",SD,",4) == 0)          //Serial Diagnost
    {
        serial_diagnost = true;
    }
    else if(memcmp(((char*)Uart1_array+1),",SP,",4) == 0)          //Input data {,SP,11111,}
    {
        NRFP_flag = true;
        token = strtok((char*)Uart1_array, ",");
        token = strtok(NULL,",");
        for(k=0; k<1; k++)
        {
            token = strtok(NULL,",");
            sprintf(token_buf[k],"%s",token);
        }
        N_Serial = (uint32_t)(atol(token_buf[0]));
    }
    else if(memcmp(((char*)Uart1_array+1),",SC,",4) == 0)          //Input data {,SC,11111,}
    {
        NRFC_flag = true;
        token = strtok((char*)Uart1_array, ",");
        token = strtok(NULL,",");
        for(k=0; k<1; k++)
        {
            token = strtok(NULL,",");
            sprintf(token_buf[k],"%s",token);
        }
        N_Serial = (uint32_t)(atol(token_buf[0]));
        
    }
}
/*=============================================================================
 * Function     : Uart2_Data_Handler.
 * Description  : UART2 received data decoding
 * Parameters   : void 
 * Return       : Void.
 * Example      : Uart2_Data_Handler();
===========================================================================================================================*/

void Uart2_Data_Handler(void)
{
    char* token,token_buf[6][10],lbuf[5];
    uint8_t k;
    
    if(memcmp(((char*)Uart2_array+1),",LP,",4) == 0)          //Output data {,OD,digital_output,mp3_stat,track_num,}
    {
        nrf_data_flag = true;
        token = strtok((char*)Uart2_array, ",");
        token = strtok(NULL,",");
        for(k=0; k<6; k++)
        {
            token = strtok(NULL,",");
            sprintf(token_buf[k],"%s",token);
        }
        memset(CAN_Txpara.CAN_Buf,0x00,8*sizeof(uint8_t));
        CAN_Rxpara.CANID = 0;
        N_Load = 0;
        N_ADC  = 0;
        CAN_Rxpara.CANID     = (uint16_t)(atol(token_buf[0]));//Hook Type Main/Aux
        N_Load     = (uint16_t)(atol(token_buf[1]));//Calibrated Load
        N_ADC      = (uint16_t)(atol(token_buf[2]));//Load Adc Count
        CAN_Txpara.CAN_Buf[4] = (uint8_t)(atol(token_buf[3]));//Load Status safe/approch/overload
        CAN_Txpara.CAN_Buf[5] = (uint8_t)(((atol(token_buf[4]))& 0xFF00)>>8);//Battery adc count
        CAN_Txpara.CAN_Buf[6] = (uint8_t)(((atol(token_buf[4]))& 0x00FF)>>0);
        CAN_Txpara.CAN_Buf[7] = (uint8_t)(atol(token_buf[5]));//temperature
        CAN_Txpara.CAN_Buf[0] = (uint8_t)((N_Load & 0xFF00)>>8);
        CAN_Txpara.CAN_Buf[1] = (uint8_t)((N_Load & 0x00FF)>>0);
        CAN_Txpara.CAN_Buf[2] = (uint8_t)((N_ADC & 0xFF00)>>8);
        CAN_Txpara.CAN_Buf[3] = (uint8_t)((N_ADC & 0x00FF)>>0);
        
    }
}
/*=============================================================================
 * Function     : Can_Data_Send.
 * Description  : send data on can bus
 * Parameters   : void 
 * Return       : Void.
 * Example      : Can_Data_Send();
===========================================================================================================================*/
void Can_Data_Send(void)
{
    static uint8_t can_frame_no = 0;
    uint16_t crc=0;
    if(can_frame_no == 0)
    {
        ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        ECAN_TxMSG.dlc    = 8;
        ECAN_TxMSG.id     = CAN_Txpara.CANID;//CAN_Txpara.CANID;
        ECAN_TxMSG.data0  = 0x01;
        ECAN_TxMSG.data1  = (uint8_t)((ADC[2]&0xFF00)>>8);
        ECAN_TxMSG.data2  = (uint8_t)((ADC[2]&0x00FF)>>0);
        ECAN_TxMSG.data3  = (uint8_t)((ADC[3]&0xFF00)>>8);
        ECAN_TxMSG.data4  = (uint8_t)((ADC[3]&0x00FF)>>0);
        ECAN_TxMSG.data5  = DADC[0];//(uint8_t)((ADC[2]&0xFF00)>>8);//119
        crc = (uint16_t)(ECAN_TxMSG.data0+ECAN_TxMSG.data1+ECAN_TxMSG.data2+ECAN_TxMSG.data3+ECAN_TxMSG.data4+ECAN_TxMSG.data5);
        crc = Check_Sum(crc);
        ECAN_TxMSG.data6 = (uint8_t)((crc&0xFF00)>>8);
        ECAN_TxMSG.data7 = (uint8_t)((crc&0x00FF)>>0);
        //ECAN_TxMSG.data6  = (uint8_t)((ADC[2]&0xFF00)>>8);//(uint8_t)((ADC[2]&0x00FF)>>0);
        //ECAN_TxMSG.data7  = (uint8_t)((ADC[2]&0x00FF)>>0);// DADC[0];
        CAN_transmit(&ECAN_TxMSG);
        //can_frame_no++;
        can_frame_no = 0;
//        CAN_Txpara.CAN_Buf[0] = 0x01;
//        CAN_Txpara.CAN_Buf[1] = (uint8_t)((ADC[0]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[2] = (uint8_t)((ADC[0]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[3] = (uint8_t)((ADC[1]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[4] = (uint8_t)((ADC[1]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[5] = (uint8_t)((ADC[2]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[6] = (uint8_t)((ADC[2]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[7] = DADC[0];
//        ECanWriteMessage(0,CAN_Txpara.CANID,8,0,CAN_Txpara.CAN_Buf);
//        can_frame_no++;
    }
    else if(can_frame_no == 1)
    {
        ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        ECAN_TxMSG.dlc    = 8;
        ECAN_TxMSG.id     = CAN_Txpara.CANID;//CAN_Txpara.CANID;
        ECAN_TxMSG.data0  = 0x02;
        ECAN_TxMSG.data1  = (uint8_t)((ADC[3]&0xFF00)>>8);
        ECAN_TxMSG.data2  = (uint8_t)((ADC[3]&0x00FF)>>0);
        ECAN_TxMSG.data3  = (uint8_t)((ADC[4]&0xFF00)>>8);
        ECAN_TxMSG.data4  = (uint8_t)((ADC[4]&0x00FF)>>0);
        ECAN_TxMSG.data5  = (uint8_t)((ADC[5]&0xFF00)>>8);
        ECAN_TxMSG.data6  = (uint8_t)((ADC[5]&0x00FF)>>0);
        ECAN_TxMSG.data7  =  DADC[1];
        CAN_transmit(&ECAN_TxMSG);
        can_frame_no++;
//        CAN_Txpara.CAN_Buf[0] = 0x02;
//        CAN_Txpara.CAN_Buf[1] = (uint8_t)((ADC[3]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[2] = (uint8_t)((ADC[3]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[3] = (uint8_t)((ADC[4]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[4] = (uint8_t)((ADC[4]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[5] = (uint8_t)((ADC[5]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[6] = (uint8_t)((ADC[5]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[7] = DADC[1];
//        ECanWriteMessage(0,CAN_Txpara.CANID,8,0,CAN_Txpara.CAN_Buf); 
//        can_frame_no++;
    }
    else if(can_frame_no == 2)
    {
        ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        ECAN_TxMSG.dlc    = 8;
        ECAN_TxMSG.id     = CAN_Txpara.CANID;//CAN_Txpara.CANID;
        ECAN_TxMSG.data0  = 0x03;
        ECAN_TxMSG.data1  = (uint8_t)((ADC[7]&0xFF00)>>8);
        ECAN_TxMSG.data2  = (uint8_t)((ADC[7]&0x00FF)>>0);
        ECAN_TxMSG.data3  = (uint8_t)((ADC[6]&0xFF00)>>8);
        ECAN_TxMSG.data4  = (uint8_t)((ADC[6]&0x00FF)>>0);
        ECAN_TxMSG.data5  = (uint8_t)((ADC[9]&0xFF00)>>8);
        ECAN_TxMSG.data6  = (uint8_t)((ADC[9]&0x00FF)>>0);
        ECAN_TxMSG.data7  =  DADC[2];
        CAN_transmit(&ECAN_TxMSG);
        can_frame_no++;
//        CAN_Txpara.CAN_Buf[0] = 0x03;
//        CAN_Txpara.CAN_Buf[1] = (uint8_t)((ADC[7]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[2] = (uint8_t)((ADC[7]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[3] = (uint8_t)((ADC[6]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[4] = (uint8_t)((ADC[6]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[5] = (uint8_t)((ADC[9]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[6] = (uint8_t)((ADC[9]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[7] = DADC[2];
//        ECanWriteMessage(0,CAN_Txpara.CANID,8,0,CAN_Txpara.CAN_Buf); 
//        can_frame_no++;
    }
    else if(can_frame_no == 3)
    {
        ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        ECAN_TxMSG.dlc    = 8;
        ECAN_TxMSG.id     = CAN_Txpara.CANID;//CAN_Txpara.CANID;
        ECAN_TxMSG.data0  = 0x04;
        ECAN_TxMSG.data1  = (uint8_t)((ADC[8]&0xFF00)>>8);
        ECAN_TxMSG.data2  = (uint8_t)((ADC[8]&0x00FF)>>0);
        ECAN_TxMSG.data3  = 0;
        ECAN_TxMSG.data4  = 0;
        ECAN_TxMSG.data5  = 0;
        ECAN_TxMSG.data6  = 0;
        ECAN_TxMSG.data7  = 0;
        CAN_transmit(&ECAN_TxMSG);
        can_frame_no = 0;
//        CAN_Txpara.CAN_Buf[0] = 0x04;
//        CAN_Txpara.CAN_Buf[1] = (uint8_t)((ADC[8]&0xFF00)>>8);
//        CAN_Txpara.CAN_Buf[2] = (uint8_t)((ADC[8]&0x00FF)>>0);
//        CAN_Txpara.CAN_Buf[3] = 0;
//        CAN_Txpara.CAN_Buf[4] = 0;
//        CAN_Txpara.CAN_Buf[5] = 0;
//        CAN_Txpara.CAN_Buf[6] = 0;
//        CAN_Txpara.CAN_Buf[7] = 0;
//        ECanWriteMessage(0,CAN_Txpara.CANID,8,0,CAN_Txpara.CAN_Buf); 
//        can_frame_no = 0;
    }
}

void CAN_Request_Send(void)
{
    //memset(ECAN_TxMSG.,0x00,8*sizeof(uint8_t));
    ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
    ECAN_TxMSG.dlc    = 8;
    ECAN_TxMSG.id     = CAN_Txpara.CANID;//CAN_Txpara.CANID;
    ECAN_TxMSG.data0  = 'S';
    ECAN_TxMSG.data1  = 'T';
    ECAN_TxMSG.data2  = 'A';
    ECAN_TxMSG.data3  = 'T';
    ECAN_TxMSG.data4  = 0;
    ECAN_TxMSG.data5  = 0;
    ECAN_TxMSG.data6  = 0;
    ECAN_TxMSG.data7  = 0;
    CAN_transmit(&ECAN_TxMSG);
//    CAN_Txpara.CAN_Buf[0] = 'S';
//    CAN_Txpara.CAN_Buf[1] = 'T';
//    CAN_Txpara.CAN_Buf[2] = 'A';
//    CAN_Txpara.CAN_Buf[3] = 'T';   
//    ECanWriteMessage(0,CAN_Txpara.CANID,8,0,CAN_Txpara.CAN_Buf);  // Put request on CAN bus
    Delay_Ms(100);
//    memset(CAN_Txpara.CAN_Buf,0x00,8*sizeof(uint8_t)); 
    Yellow_led = false;
    can_timeout = 10000;
}
/*=============================================================================
 * Function     : Can_to_Uart_Data.
 * Description  : Receive data from CAN & send to UART1
 * Parameters   : void 
 * Return       : Void.
 * Example      : Can_to_Uart_Data();
===========================================================================================================================*/

//void Can_to_Uart_Data(void)
//{
//    char disp[25]={0};
//    
//    sprintf(disp,"{,CD,%d,%d,%d,%d,%d",(uint16_t)CAN_Rxpara.CANID,CAN_Rxpara.CAN_Buf[0],CAN_Rxpara.CAN_Buf[1],CAN_Rxpara.CAN_Buf[2],CAN_Rxpara.CAN_Buf[3]); 
//    EUSART1_String(disp); 
//    sprintf(disp,",%d,%d,%d,%d,}\n",CAN_Rxpara.CAN_Buf[4],CAN_Rxpara.CAN_Buf[5],CAN_Rxpara.CAN_Buf[6],CAN_Rxpara.CAN_Buf[7]); 
//    EUSART1_String(disp);
//}
/*=============================================================================
 * Function     : Data_Process.
 * Description  : Process data from inputs & control outputs
 * Parameters   : void 
 * Return       : Void.
 * Example      : Data_Process();
===========================================================================================================================*/

void Data_Process(void)
{
    uint8_t channel = 0;
    uint8_t eep_read1=0,eep_read2=0,eep_read3=0;
    char temp_buf[20]={0};
    float F_temp=0.0;
    memset(temp_ADC,0x00,15*sizeof(uint16_t));           // Clear buffer
    memset(DADC,0x00,3*sizeof(uint16_t));
    MC33972_Read_ADC(Analog_Ch0);                   // Set MSDI analog channel
    Delay_Ms(1);
    ADC[0] = Get_Adc_Data(3,250);                   // Read voltage
    ADC[0] = (uint16_t)((ADC[0]*0.00735)*10);
    Delay_Ms(1);
    for(channel=7;channel<22;channel++)            // Read Multiple Analog Channel
    {
        MC33972_Read_ADC(Analog_Ch0+channel);
        Delay_Ms(1);
        temp_ADC[channel-7]=Get_Adc_Data(3,100);                // Store adc value to ADC buffer from 0 location
    }
        
    F_temp = (float)(temp_ADC[5]/0.85); //12
    F_temp/=10;
    ADC[1] = (uint16_t)F_temp; //12
   
    //ADC[0] for voltage
    //ADC[1] for temperature
    //ADC[2] for P1
    //ADC[3] for P2
    //ADC[4] for P3
    //ADC[5] for P4
    //ADC[6] for Angle1
    //ADC[7] for Length1
    //ADC[8] for Angle2
    //ADC[9] for Length2
    /////////////////////////////////////////////
    //Pressure 1
    if(temp_ADC[4]>(ADC[2]+ADC_Offset))
    {
        ADC_PCnt[0]+=1;
        if(ADC_PCnt[0]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[2] = temp_ADC[4];
            ADC_PCnt[0] = 0;
        }
    }
    else if(ADC[2]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[4]<(ADC[2]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[0]+=1;
            if(ADC_NCnt[0]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[2] = temp_ADC[4];
               ADC_NCnt[0] = 0; 
            }
        }
        else
        {
           ADC[2] = temp_ADC[4];   // update value
        }
    }
    else
    {
        ADC[2] = temp_ADC[4]; // update value
    }
    /////////////////////////////////////////////
    //Pressure 2
    if(temp_ADC[3]>(ADC[3]+ADC_Offset))
    {
        ADC_PCnt[1]+=1;
        if(ADC_PCnt[1]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[3] = temp_ADC[3];
            ADC_PCnt[1] = 0;
        }
    }
    else if(ADC[3]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[3]<(ADC[3]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[1]+=1;
            if(ADC_NCnt[1]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[3] = temp_ADC[3];
               ADC_NCnt[1] = 0; 
            }
        }
        else
        {
           ADC[3] = temp_ADC[3];   // update value
        }
    }
    else
    {
        ADC[3] = temp_ADC[3]; // update value
    }
    ///////////////////////////////////////////////////////
    //Pressure 3
    if(temp_ADC[2]>(ADC[4]+ADC_Offset))
    {
        ADC_PCnt[2]+=1;
        if(ADC_PCnt[2]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[4] = temp_ADC[2];
            ADC_PCnt[2] = 0;
        }
    }
    else if(ADC[4]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[2]<(ADC[4]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[2]+=1;
            if(ADC_NCnt[2]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[4] = temp_ADC[2];
               ADC_NCnt[2] = 0; 
            }
        }
        else
        {
           ADC[4] = temp_ADC[2];   // update value
        }
    }
    else
    {
        ADC[4] = temp_ADC[2]; // update value
    }
    ///////////////////////////////////////////////////////
    //Pressure 4
    if(temp_ADC[1]>(ADC[5]+ADC_Offset))
    {
        ADC_PCnt[3]+=1;
        if(ADC_PCnt[3]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[5] = temp_ADC[1];
            ADC_PCnt[3] = 0;
        }
    }
    else if(ADC[5]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[1]<(ADC[5]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[3]+=1;
            if(ADC_NCnt[3]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[5] = temp_ADC[1];
               ADC_NCnt[3] = 0; 
            }
        }
        else
        {
           ADC[5] = temp_ADC[1];   // update value
        }
    }
    else
    {
        ADC[5] = temp_ADC[1]; // update value
    }
    //////////////////////////////////////////////////////
    // Angle 1
    if(temp_ADC[0]>(ADC[6]+ADC_Offset))
    {
        ADC_PCnt[4]+=1;
        if(ADC_PCnt[4]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[6] = temp_ADC[0];
            ADC_PCnt[4] = 0;
        }
    }
    else if(ADC[6]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[0]<(ADC[6]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[4]+=1;
            if(ADC_NCnt[4]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[6] = temp_ADC[0];
               ADC_NCnt[4] = 0; 
            }
        }
        else
        {
           ADC[6] = temp_ADC[0];   // update value
        }
    }
    else
    {
        ADC[6] = temp_ADC[0]; // update value
    }
    //////////////////////////////////////////////////
    // Length 1
    if(temp_ADC[7]>(ADC[7]+ADC_Offset))
    {
        ADC_PCnt[5]+=1;
        if(ADC_PCnt[5]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[7] = temp_ADC[7];
            ADC_PCnt[5] = 0;
        }
    }
    else if(ADC[7]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[7]<(ADC[7]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[5]+=1;
            if(ADC_NCnt[5]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[7] = temp_ADC[7];
               ADC_NCnt[5] = 0; 
            }
        }
        else
        {
           ADC[7] = temp_ADC[7];   // update value
        }
    }
    else
    {
        ADC[7] = temp_ADC[7]; // update value
    }
    //////////////////////////////////////////////////
    //Angle 2
    if(temp_ADC[8]>(ADC[8]+ADC_Offset))
    {
        ADC_PCnt[6]+=1;
        if(ADC_PCnt[6]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[8] = temp_ADC[8];
            ADC_PCnt[6] = 0;
        }
    }
    else if(ADC[8]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[8]<(ADC[8]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[6]+=1;
            if(ADC_NCnt[6]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[8] = temp_ADC[8];
               ADC_NCnt[6] = 0; 
            }
        }
        else
        {
           ADC[8] = temp_ADC[8];   // update value
        }
    }
    else
    {
        ADC[8] = temp_ADC[8]; // update value
    }
    //////////////////////////////////////////////////
    // Length 2
    if(temp_ADC[9]>(ADC[9]+ADC_Offset))
    {
        ADC_PCnt[7]+=1;
        if(ADC_PCnt[7]>ADC_Offset_Count)
        {// after ADC_Offset_Count also value is increased then only update value
            ADC[9] = temp_ADC[9];
            ADC_PCnt[7] = 0;
        }
    }
    else if(ADC[9]>ADC_Offset)  // for less value first make sure that ADC value is greater than offset
    {
        if(temp_ADC[9]<(ADC[9]-ADC_Offset)) //if adc value sudden decrease then wait & check upto ADC_Offset_Count time
        {
            ADC_NCnt[7]+=1;
            if(ADC_NCnt[7]>ADC_Offset_Count)
            {// after ADC_Offset_Count also value is decrease then only update value
               ADC[9] = temp_ADC[9];
               ADC_NCnt[7] = 0; 
            }
        }
        else
        {
           ADC[9] = temp_ADC[9];   // update value
        }
    }
    else
    {
        ADC[9] = temp_ADC[9]; // update value
    }
    //////////////////////////////////////////////////
    if((temp_ADC[10]>Wire_Break_min) && (temp_ADC[10]<Wire_Break_max))  //ADC[4]           
    {
       DADC[2] |=0x01; //8
    }
    else if((temp_ADC[10]>Switch_Open_Min) && (temp_ADC[10]<Switch_Open_Max)) ////ADC[4]  
    {
        DADC[2] &=~0x01; //8
        DADC[0] &=~0x01; //4
        DADC[1] &=~0x01; //5
    }
    else if(temp_ADC[10]<Switch_Close_Ground)
    {
        DADC[0] |=0x01;
    }
    else if(temp_ADC[10]>Switch_Close_Battery)
    {
       DADC[1] |=0x01; 
    }
    
    if((temp_ADC[11]>Wire_Break_min) && (temp_ADC[11]<Wire_Break_max))  //ADC[5]
    {
       DADC[2] |=0x02;
    }
    else if((temp_ADC[11]>Switch_Open_Min) && (temp_ADC[11]<Switch_Open_Max)) //ADC[5]
    {
        DADC[2] &=~0x02;
        DADC[0] &=~0x02;
        DADC[1] &=~0x02;
    }
    else if(temp_ADC[11]<Switch_Close_Ground)
    {
        DADC[0] |=0x02;
    }
    else if(temp_ADC[11]>Switch_Close_Battery)
    {
        DADC[1] |=0x02;
    }
    
    if((temp_ADC[12]>Wire_Break_min) && (temp_ADC[12]<Wire_Break_max))  //ADC[6]
    {
        DADC[2] |=0x04;
    }
    else if((temp_ADC[12]>Switch_Open_Min) && (temp_ADC[12]<Switch_Open_Max)) //ADC[6]
    {
        DADC[2] &=~0x04;
        DADC[0] &=~0x04;
        DADC[1] &=~0x04;
    }
    else if(temp_ADC[12]<Switch_Close_Ground)
    {
        DADC[0] |=0x04;
    }
    else if(temp_ADC[12]>Switch_Close_Battery)
    {
        DADC[1] |=0x04;
    }
    
    if((temp_ADC[13]>Wire_Break_min) && (temp_ADC[13]<Wire_Break_max))  //ADC[7]
    {
        DADC[2] |=0x08;
    }
    else if((temp_ADC[13]>Switch_Open_Min) && (temp_ADC[13]<Switch_Open_Max)) //ADC[7]
    {
        DADC[2] &=~0x08;
        DADC[0] &=~0x08;
        DADC[1] &=~0x08;
    }
   else if(temp_ADC[13]<Switch_Close_Ground)
    {
        DADC[0] |=0x08;
    }
    else if(temp_ADC[13]>Switch_Close_Battery)
    {
        DADC[1] |=0x08;
    }
    
    if((temp_ADC[14]>Wire_Break_min) && (temp_ADC[14]<Wire_Break_max))  //ADC[7]
    {
        DADC[2] |=0x10;
    }
    else if((temp_ADC[14]>Switch_Open_Min) && (temp_ADC[14]<Switch_Open_Max)) //ADC[7]
    {
        DADC[2] &=~0x10;
        DADC[0] &=~0x10;
        DADC[1] &=~0x10;
    }
   else if(temp_ADC[14]<Switch_Close_Ground)
    {
        DADC[0] |=0x10;
    }
    else if(temp_ADC[14]>Switch_Close_Battery)
    {
        DADC[1] |=0x10;
    }
    
    if(can_timeout == 0)         // Checking whether Master on CAN Bus is connected or not
    {
        CAN_Request_Send();
    }
    
    if(Check_CAN_Interrupt()!=0)  // Check CAN interrupt arrived or not
    {
        if(CAN_messagesInBuffer()!=0) // check CAN message buffer is not empty
        {
            if(CAN_receive(&ECAN_RxMSG)!=0) // Read received data on CAN
           {
               CAN_Rxpara.CANID      = ECAN_RxMSG.id;
               CAN_Rxpara.CAN_Buf[0] = ECAN_RxMSG.data0;
               CAN_Rxpara.CAN_Buf[1] = ECAN_RxMSG.data1;
               CAN_Rxpara.CAN_Buf[2] = ECAN_RxMSG.data2;
               CAN_Rxpara.CAN_Buf[3] = ECAN_RxMSG.data3;
               CAN_Rxpara.CAN_Buf[4] = ECAN_RxMSG.data4;
               CAN_Rxpara.CAN_Buf[5] = ECAN_RxMSG.data5;
               CAN_Rxpara.CAN_Buf[6] = ECAN_RxMSG.data6;
               CAN_Rxpara.CAN_Buf[7] = ECAN_RxMSG.data7;
               CAN_RStatus = 1;          // if new data is present then set can flag
           }
        }   
    }
    //CAN_RStatus = Read_ECan(0,&CAN_Rxpara.CANID,8,0,CAN_Rxpara.CAN_Buf);    // Read CAN Bus
    if(CAN_RStatus == 1)            // Check data on CAN is present
    {
        if(CAN_Rxpara.CANID == CAN_Txpara.CANID)//CAN_Txpara.CANID)  // Check CAN ID
        {
            if((CAN_Rxpara.CAN_Buf[0]=='O') && (CAN_Rxpara.CAN_Buf[1]=='D')) // Check OD String
            {
                digital_output = (uint16_t)((CAN_Rxpara.CAN_Buf[2]<<8)|(CAN_Rxpara.CAN_Buf[3]<<0));
                Digital_Output_Handler();  // Take action as per output signal
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='D') && (CAN_Rxpara.CAN_Buf[1]=='M'))
            {
               Red_Led = true; 
               serial_diagnost = true;
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='N') && (CAN_Rxpara.CAN_Buf[1]=='M'))
            {
               Red_Led = false; 
               serial_diagnost = false;
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='O') && (CAN_Rxpara.CAN_Buf[1]=='K')) // Check Valid Response
            {
                Yellow_led = true;  
                can_timeout = 60000;
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='T') && (CAN_Rxpara.CAN_Buf[1]=='M'))  // MAIN 
            {
               // Eeprom_Write(2,1);
                Eeprom_Write(1,'1');Eeprom_Write(2,'1');Eeprom_Write(3,'1');
                CLRWDT();
                Delay_Ms(100);
                eep_read1 = Eeprom_Read(1); eep_read2 = Eeprom_Read(2); eep_read3 = Eeprom_Read(3);
                if(((eep_read1 != '1')&&(eep_read2 != '1'))||((eep_read2 != '1')&&(eep_read3 != '1'))||((eep_read1 != '1')&&(eep_read3 != '1')))
                {
                    Eeprom_Write(1,'1');Eeprom_Write(2,'1');Eeprom_Write(3,'1');
                    EUSART1_String("CANID 409 stored at eeprom failed\r\n");
                }
                else
                {
                    EUSART1_String("CANID 409 stored at eeprom Success\r\n");
                }
                CAN_Txpara.CANID = 409;
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='T') && (CAN_Rxpara.CAN_Buf[1]=='A'))  // AUX
            {
                //Eeprom_Write(2,2);
                Eeprom_Write(1,'2');Eeprom_Write(2,'2');Eeprom_Write(3,'2');
                CLRWDT();
                Delay_Ms(100);
                eep_read1 = Eeprom_Read(1); eep_read2 = Eeprom_Read(2); eep_read3 = Eeprom_Read(3);
                if(((eep_read1 != '2')&&(eep_read2 != '2'))||((eep_read2 != '2')&&(eep_read3 != '2'))||((eep_read1 != '2')&&(eep_read3 != '2')))
                {
                    Eeprom_Write(1,'2');Eeprom_Write(2,'2');Eeprom_Write(3,'2');
                    EUSART1_String("CANID 410 stored at eeprom failed\r\n");
                }
                else
                {
                    EUSART1_String("CANID 410 stored at eeprom Success\r\n");
                }
                CAN_Txpara.CANID = 410;
            }
            else if((CAN_Rxpara.CAN_Buf[0]=='C') && (CAN_Rxpara.CAN_Buf[1]=='S'))  // CAN Speed 
            {
                if(CAN_Rxpara.CAN_Buf[2] == 1)
                {
                   //Eeprom_Write(1,1);
                    Eeprom_Write(4,'1');Eeprom_Write(5,'1');Eeprom_Write(6,'1');
                    CLRWDT();
                    Delay_Ms(100);
                    eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5); eep_read3 = Eeprom_Read(6);
                    if(((eep_read1 != '1')&&(eep_read2 != '1'))||((eep_read2 != '1')&&(eep_read3 != '1'))||((eep_read1 != '1')&&(eep_read3 != '1')))
                    {
                        Eeprom_Write(4,'1');Eeprom_Write(5,'1');Eeprom_Write(6,'1');
                        EUSART1_String("CANSpeed 100K stored at eeprom failed\r\n");
                    }
                    else
                    {
                        EUSART1_String("CANSpeed 100k stored at eeprom Success\r\n");
                    }
                }
                else if(CAN_Rxpara.CAN_Buf[2] == 2)
                {
                   //Eeprom_Write(1,2); 
                    Eeprom_Write(4,'2');Eeprom_Write(5,'2');Eeprom_Write(6,'2');
                    CLRWDT();
                    Delay_Ms(100);
                    eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5); eep_read3 = Eeprom_Read(6);
                    if(((eep_read1 != '2')&&(eep_read2 != '2'))||((eep_read2 != '2')&&(eep_read3 != '2'))||((eep_read1 != '2')&&(eep_read3 != '2')))
                    {
                        Eeprom_Write(4,'2');Eeprom_Write(5,'2');Eeprom_Write(6,'2');
                        EUSART1_String("CANSpeed 100K stored at eeprom failed\r\n");
                    }
                    else
                    {
                        EUSART1_String("CANSpeed 100k stored at eeprom Success\r\n");
                    }
                }
                else if(CAN_Rxpara.CAN_Buf[2] == 3)
                {
                   //Eeprom_Write(1,3); 
                    Eeprom_Write(4,'3');Eeprom_Write(5,'3');Eeprom_Write(6,'3');
                    CLRWDT();
                    Delay_Ms(100);
                    eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5); eep_read3 = Eeprom_Read(6);
                    if(((eep_read1 != '3')&&(eep_read2 != '3'))||((eep_read2 != '3')&&(eep_read3 != '3'))||((eep_read1 != '3')&&(eep_read3 != '3')))
                    {
                        Eeprom_Write(4,'3');Eeprom_Write(5,'3');Eeprom_Write(6,'3');
                        EUSART1_String("CANSpeed 100K stored at eeprom failed\r\n");
                    }
                    else
                    {
                        EUSART1_String("CANSpeed 100k stored at eeprom Success\r\n");
                    }
                }
                else if(CAN_Rxpara.CAN_Buf[2] == 4)
                {
                  // Eeprom_Write(1,4); 
                    Eeprom_Write(4,'4');Eeprom_Write(5,'4');Eeprom_Write(6,'4');
                    CLRWDT();
                    Delay_Ms(100);
                    eep_read1 = Eeprom_Read(4); eep_read2 = Eeprom_Read(5); eep_read3 = Eeprom_Read(6);
                    if(((eep_read1 != '4')&&(eep_read2 != '4'))||((eep_read2 != '4')&&(eep_read3 != '4'))||((eep_read1 != '4')&&(eep_read3 != '4')))
                    {
                        Eeprom_Write(4,'4');Eeprom_Write(5,'4');Eeprom_Write(6,'4');
                        EUSART1_String("CANSpeed 100K stored at eeprom failed\r\n");
                    }
                    else
                    {
                        EUSART1_String("CANSpeed 100k stored at eeprom Success\r\n");
                    }
                }
            }
            else
            {
                Yellow_led = false;  
            }
        }
        CAN_RStatus = 0;
        memset(CAN_Rxpara.CAN_Buf,0x00,8*sizeof(uint16_t)); 
    }
    if(Uart1_Frame_Flag == 1)
    {
        //EUSART1_String("uart flag\n");
        Uart1_Data_Handler();            // Check UART received data
        if(uart1_data_flag == true)
        {
            Digital_Output_Handler();   // Control OUTPUTS
            //EUSART1_String("Digital output\n");
            uart1_data_flag = false;
        }
        Uart1_Frame_Flag = 0;
    }
    if(Uart2_Frame_Flag == 1)
    {
        Uart2_Data_Handler();           // Check UART received data
        if(uart2_data_flag == true)
        {
          Uart1_Data_Send();            //uart2 received data send to uart1
          uart2_data_flag = false;
        }
        if(nrf_data_flag == 1)
        {
//            ECAN_TxMSG.idType = dSTANDARD_CAN_MSG_ID_2_0B;
//            ECAN_TxMSG.dlc    = 8;
//            ECAN_TxMSG.id     = CAN_Rxpara.CANID;
//            ECAN_TxMSG.data0  = CAN_Txpara.CAN_Buf[0];
//            ECAN_TxMSG.data1  = CAN_Txpara.CAN_Buf[1];
//            ECAN_TxMSG.data2  = CAN_Txpara.CAN_Buf[2];
//            ECAN_TxMSG.data3  = CAN_Txpara.CAN_Buf[3];
//            ECAN_TxMSG.data4  = CAN_Txpara.CAN_Buf[4];
//            ECAN_TxMSG.data5  = CAN_Txpara.CAN_Buf[5];
//            ECAN_TxMSG.data6  = CAN_Txpara.CAN_Buf[6];
//            ECAN_TxMSG.data7  = CAN_Txpara.CAN_Buf[7];
//            CAN_transmit(&ECAN_TxMSG);
            //ECanWriteMessage(0,CAN_Rxpara.CANID,8,0,CAN_Txpara.CAN_Buf);
//            for(channel=0;channel<38;channel++)
//            {
//                EUSART1_Write(Uart2_array[channel]);
//            }
//            EUSART1_String("\n");
            if(serial_diagnost == true)
            {
              EUSART1_String("{,LP,");
              sprintf(temp_buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,}\n",(uint16_t)CAN_Rxpara.CANID,CAN_Txpara.CAN_Buf[0],CAN_Txpara.CAN_Buf[1],CAN_Txpara.CAN_Buf[2],CAN_Txpara.CAN_Buf[3],CAN_Txpara.CAN_Buf[4],CAN_Txpara.CAN_Buf[5],CAN_Txpara.CAN_Buf[6],CAN_Txpara.CAN_Buf[7]);  
              EUSART1_String(temp_buf);
            }
            memset(CAN_Txpara.CAN_Buf,0x00,8*sizeof(uint8_t));
            nrf_data_flag = false;
        }
        Uart2_Frame_Flag = 0;
    }
    
    if(NRFP_flag == 1)
    {
        EUSART2_String("#SN");
        EUSART2_Write((uint8_t)(N_Serial/10000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%10000)/1000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%1000)/100)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%100)/10)+0x30);
        EUSART2_Write((uint8_t)(N_Serial%10)+0x30);
        EUSART2_Write('\n');
        NRFP_flag = false;
    }
    else if(NRFC_flag == 1)
    {
        EUSART2_String("B4");
        EUSART2_Write((uint8_t)(N_Serial/10000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%10000)/1000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%1000)/100)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%100)/10)+0x30);
        EUSART2_Write((uint8_t)(N_Serial%10)+0x30);
        EUSART2_Write('\n');
        Delay_Ms(100);
        CLRWDT();
        EUSART2_String("B4");
        EUSART2_Write((uint8_t)(N_Serial/10000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%10000)/1000)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%1000)/100)+0x30);
        EUSART2_Write((uint8_t)((N_Serial%100)/10)+0x30);
        EUSART2_Write((uint8_t)(N_Serial%10)+0x30);
        EUSART2_Write('\n');
        
//        EUSART1_String("B4");
//        EUSART1_Write((uint8_t)(N_Serial/10000)+0x30);
//        EUSART1_Write((uint8_t)((N_Serial%10000)/1000)+0x30);
//        EUSART1_Write((uint8_t)((N_Serial%1000)/100)+0x30);
//        EUSART1_Write((uint8_t)((N_Serial%100)/10)+0x30);
//        EUSART1_Write((uint8_t)(N_Serial%10)+0x30);
//        EUSART1_Write('\n');
        NRFC_flag = false;
    }
}

/*=============================================================================
 * Function     : Uart1_Data_Send.
 * Description  : Send data on UART1
 * Parameters   : void 
 * Return       : Void.
 * Example      : Uart1_Data_Send();
===========================================================================================================================*/

void Uart1_Data_Send(void)
{
    char disp[25]={0};

    sprintf(disp,"{,AD,%d,%d,%d,%d,%d,",ADC[0],ADC[1],ADC[2],ADC[3],ADC[4]);
    EUSART1_String(disp);
    sprintf(disp,"%d,%d,%d,%d,%d,",ADC[5],ADC[7],ADC[6],ADC[9],ADC[8]); 
    EUSART1_String(disp);
    sprintf(disp,"%d,%d,%d,}\n",DADC[0],DADC[1],DADC[2]); 
    EUSART1_String(disp);

}

/*=============================================================================
 * Function     : EUSART1_Receive_ISR.
 * Description  : UART1 Interrupt Routine
 * Parameters   : void 
 * Return       : Void.
 * Example      : EUSART1_Receive_ISR(); must be call in interrupt routine only
===========================================================================================================================*/

void EUSART1_Receive_ISR(void)
{
    uint8_t RxdData=0;
    RxdData = RCREG1;
    if(uart1_index>24)
    {
        uart1_index = 0;
    }
    else if(RxdData != '\n' && uart1_index<25)
    {
        Uart1_array[uart1_index++] = RxdData;
    }
    else if(RxdData == '\n')
    {
        Uart1_array[uart1_index++] = RxdData;
        Uart1_Frame_Flag = 1;
        uart1_index = 0;
    }
}

/*=============================================================================
 * Function     : EUSART2_Receive_ISR.
 * Description  : UART Interrupt Routine
 * Parameters   : void 
 * Return       : Void.
 * Example      : EUSART2_Receive_ISR(); must be call in interrupt routine only
===========================================================================================================================*/

void EUSART2_Receive_ISR(void)
{
    uint8_t RxdData = 0;
    RxdData = RCREG2;
    if(uart2_index>38)
    {
        uart2_index = 0;
    }
    else if(RxdData != '\n' && uart2_index<38)
    {
        Uart2_array[uart2_index++] = RxdData;
    }
    else if(RxdData == '\n')
    {
        Uart2_array[uart2_index++] = RxdData;
        Uart2_Frame_Flag = 1;
        uart2_index = 0;
    }
}

/*=============================================================================
 * Function     : Digital_Output_Handler.
 * Description  : Take action on digital outputs
 * Parameters   : void 
 * Return       : Void.
 * Example      : Digital_Output_Handler();
===========================================================================================================================*/

void Digital_Output_Handler(void)
{
    // Extract output data form variable
    Doutput.Output_1 = (uint8_t)((digital_output & 0x0001)>>0);
    Doutput.Output_2 = (uint8_t)((digital_output & 0x0002)>>1);
    Doutput.Output_3 = (uint8_t)((digital_output & 0x0004)>>2);
    Doutput.Output_4 = (uint8_t)((digital_output & 0x0008)>>3);
    
    Doutput.Output_5 = (uint8_t)((digital_output & 0x0010)>>4);
    Doutput.Output_6 = (uint8_t)((digital_output & 0x0020)>>5);
    Doutput.Output_7 = (uint8_t)((digital_output & 0x0040)>>6);
    Doutput.Output_8 = (uint8_t)((digital_output & 0x0080)>>7);
    
    Doutput.Output_9 = (uint8_t)((digital_output & 0x0100)>>8);
    Doutput.Output_10 = (uint8_t)((digital_output & 0x0200)>>9);
    Doutput.Output_11 = (uint8_t)((digital_output & 0x0400)>>10);
    Doutput.Output_12 = (uint8_t)((digital_output & 0x0800)>>11);
    
    Doutput.Output_13 = (uint8_t)((digital_output & 0x1000)>>12);
    Doutput.Output_14 = (uint8_t)((digital_output & 0x2000)>>13);
    Doutput.Output_15 = (uint8_t)((digital_output & 0x4000)>>14);
    Doutput.Output_16 = (uint8_t)((digital_output & 0x8000)>>15);
    //EUSART1_String("Output handler\n");
    // Take action on digital output
    switch(Doutput.Output_1)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(1,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(1,0);
            break;
    } 
    switch(Doutput.Output_2)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(2,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(2,0);
            break;
    } 
    switch(Doutput.Output_3)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(3,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(3,0);
            break;
    } 
    switch(Doutput.Output_4)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(4,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(4,0);
            break;
    } 
    switch(Doutput.Output_5)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(5,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(5,0);
            break;
    } 
    switch(Doutput.Output_6)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(6,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(6,0);
            break;
    } 
    switch(Doutput.Output_7)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(7,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(7,0);
            break;
    } 
    switch(Doutput.Output_8)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(8,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(8,0);
            break;
    } 
    switch(Doutput.Output_9)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(9,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(9,0);
            break;
    } 
    switch(Doutput.Output_10)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(10,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(10,0);
            break;
    } 
    switch(Doutput.Output_11)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(11,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(11,0);
            break;
    } 
    switch(Doutput.Output_12)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(12,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(12,0);
            break;
    } 
    switch(Doutput.Output_13)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(13,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(13,0);
            break;
    } 
    switch(Doutput.Output_14)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(14,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(14,0);
            break;
    } 
    switch(Doutput.Output_15)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(15,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(15,0);
            break;
    } 
    switch(Doutput.Output_16)
    {
        case OUTPUT_ON:
            MC33996_Digitalwrite(16,1);
            break;
        case OUTPUT_OFF:
            MC33996_Digitalwrite(16,0);
            break;
    } 
     
}
