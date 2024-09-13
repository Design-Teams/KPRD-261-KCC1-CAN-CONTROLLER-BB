#include "main.h"
// Function to initialize moving average filter buffer
void ADC_Filter_Init(uint8_t Channel_no)
 {
    // Fill buffer to 0 value initially
    for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++) 
	{
        moving_flt[Channel_no].adc_buffer[i] = 0;  
    }
    // Reset buffer index
    moving_flt[Channel_no].buffer_index = 0;
    // reset adc_sum variable
    moving_flt[Channel_no].adc_sum = 0;
}

// Function to filter ADC values using a moving average filter
uint16_t ADC_Filter(uint16_t new_value,uint8_t Channel_no) 
{
    // Subtract the old value from the sum
    moving_flt[Channel_no].adc_sum -= moving_flt[Channel_no].adc_buffer[moving_flt[Channel_no].buffer_index];
    // Add the new value to the buffer and sum
    moving_flt[Channel_no].adc_buffer[moving_flt[Channel_no].buffer_index] = new_value;
    moving_flt[Channel_no].adc_sum += new_value;
    // Move to the next buffer index
    moving_flt[Channel_no].buffer_index = ((moving_flt[Channel_no].buffer_index + 1) % FILTER_BUFFER_SIZE);
    
    // Return the average of the values in the buffer
    return (uint16_t)(moving_flt[Channel_no].adc_sum / FILTER_BUFFER_SIZE);
}

uint16_t ADC_Threshold_Check(uint16_t new_value,uint8_t Channel_no)
{
    if(new_value>(moving_flt[Channel_no].prev_adc_sum+ADC_Threshold))
    {
       moving_flt[Channel_no].ADC_PCnt++;
       if(moving_flt[Channel_no].ADC_PCnt>ADC_Change_Count)
       {
          moving_flt[Channel_no].prev_adc_sum = ADC_Filter(new_value,Channel_no);
          moving_flt[Channel_no].ADC_PCnt = 0;
       }
    }
    else if(new_value>ADC_Threshold)
    {
        if(new_value<(moving_flt[Channel_no].prev_adc_sum-ADC_Threshold))
        {
          moving_flt[Channel_no].ADC_NCnt++;
          if(moving_flt[Channel_no].ADC_NCnt>ADC_Change_Count)
          {
            moving_flt[Channel_no].prev_adc_sum = ADC_Filter(new_value,Channel_no);
            moving_flt[Channel_no].ADC_NCnt = 0;  
          }
        }
        else
        {
          moving_flt[Channel_no].prev_adc_sum = ADC_Filter(new_value,Channel_no);
          moving_flt[Channel_no].ADC_NCnt = 0;  
        }
    }
    else
    {
        moving_flt[Channel_no].prev_adc_sum = ADC_Filter(new_value,Channel_no);
        moving_flt[Channel_no].ADC_PCnt = 0;
        moving_flt[Channel_no].ADC_NCnt = 0;
    }
    return(moving_flt[Channel_no].prev_adc_sum);
}


//uint16_t Dadc_filter(uint16_t new_sample,uint8_t Channel_no) 
//{
//    // Subtract the oldest sample from the sum
//    mov_flt[Channel_no].Dsum -= mov_flt[Channel_no].Dadc_buffer[mov_flt[Channel_no].Dbuffer_index];
//    //Dsum -= Dadc_buffer[Dbuffer_index];
//    
//    // Add the new sample to the buffer and the sum
//    mov_flt[Channel_no].Dadc_buffer[mov_flt[Channel_no].Dbuffer_index] = new_sample;
//    //Dadc_buffer[Dbuffer_index] = new_sample;
//    mov_flt[Channel_no].Dsum += new_sample;
//    //Dsum += new_sample;
//    
//    // Update the index to point to the next position in the circular buffer
//    mov_flt[Channel_no].Dbuffer_index =  (mov_flt[Channel_no].Dbuffer_index+1)%5;
//    //Dbuffer_index = (Dbuffer_index + 1) % 5;
//    
//    // Return the average of the samples
//    //return (uint16_t)(Dsum / 5);
//    return (uint16_t)(mov_flt[Channel_no].Dsum/5);
//}