#include "main.h"
/*============================================================================
 * Function      :        CRC16_calculate.
 * Description   :        To calculate the CRC.
 * Parameters    :        Sum_data sum of all data (maximum sum <16 bit).
 * Return        :        CRC16-Calculated CRC.
============================================================================*/
uint16_t CRC16_calculate(uint16_t const Sum_data)
{
    uint16_t CRC16=0;
    
    CRC16 = ((0xffff-Sum_data)+ 1);
    
    return CRC16;
}

uint32_t CRC32_calculate(uint32_t const Sum_data)
{
    uint32_t CRC32=0;
    
    CRC32 = ((4294967295-Sum_data)+ 1);
    
    return CRC32;
}