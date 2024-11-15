/* 
 * File:   CRC.h
 * Author: rahul.chavan
 *
 * Created on August 30, 2024, 3:09 PM
 */

#ifndef CRC_H
#define	CRC_H

/*============================================================================
 * Function      :        CRC16_calculate.
 * Description   :        To calculate the CRC.
 * Parameters    :        Sum_data sum of all data (maximum sum <16 bit).
 * Return        :        CRC16-Calculated CRC.
============================================================================*/
uint16_t CRC16_calculate(uint16_t const Sum_data);

/*============================================================================
 * Function      :        CRC32_calculate.
 * Description   :        To calculate the CRC.
 * Parameters    :        Sum_data sum of all data (maximum sum <32 bit).
 * Return        :        CRC32-Calculated CRC.
============================================================================*/
uint32_t CRC32_calculate(uint32_t const Sum_data);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CRC_H */

