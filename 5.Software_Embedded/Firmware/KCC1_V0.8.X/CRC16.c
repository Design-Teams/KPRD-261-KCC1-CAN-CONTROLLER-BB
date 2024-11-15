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


