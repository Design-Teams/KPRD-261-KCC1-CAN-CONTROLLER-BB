#include "main.h"
/*============================================================================*/
/*
 * Function      :        Check_Sum.
 * Description   :        To calculate the checksum.
 * Parameters    :        cmd-Command, fb-feedback, pmsb-ParameterMSB and ParameterLSB.
 * Return        :        cs-Calculated checksum.
*/
/*============================================================================*/
uint16_t CRC16_calculate(uint16_t const Sum_data)
{
    uint16_t CRC16=0;
    
    CRC16 = ((0xffff-Sum_data)+ 1);
    
    return CRC16;
}

//uint32_t CRC32_calculate(uint32_t const Sum_data)
//{
//    uint32_t CRC32=0;
//    
//    CRC32 = ((4294967295-Sum_data)+ 1);
//    
//    return CRC32;
//}