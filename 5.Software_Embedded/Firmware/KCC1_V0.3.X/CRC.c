#include "main.h"
/*============================================================================*/
/*
 * Function      :        Check_Sum.
 * Description   :        To calculate the checksum.
 * Parameters    :        cmd-Command, fb-feedback, pmsb-ParameterMSB and ParameterLSB.
 * Return        :        cs-Calculated checksum.
*/
/*============================================================================*/
uint16_t Check_Sum(uint16_t CRC_data)
{
    uint16_t cs;
    
    cs = ((0xffff-CRC_data)+ 1);
    
    return cs;
}