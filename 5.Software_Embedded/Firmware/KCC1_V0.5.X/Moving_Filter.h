
#define FILTER_BUFFER_SIZE 10

#define Channel_Count 8
#define ADC_Threshold 80
#define ADC_Change_Count 5

//static uint16_t Dadc_buffer[5];
//static uint8_t Dbuffer_index = 0;
//static uint32_t Dsum = 0;

//typedef struct
//{
//	uint8_t Dbuffer_index;
//    uint16_t Dadc_buffer[5]; 
//    uint16_t Dprev_adc_sum;
//	uint32_t Dsum;
//	
//}DFilter;
//
//DFilter mov_flt[5];

typedef struct
{
	uint8_t buffer_index;
    uint8_t ADC_PCnt;
    uint8_t ADC_NCnt;
	uint16_t adc_buffer[FILTER_BUFFER_SIZE]; 
    uint16_t prev_adc_sum;
	uint32_t adc_sum;
	
}Filter;

Filter moving_flt[Channel_Count];

void ADC_Filter_Init(uint8_t Channel_no);
uint16_t ADC_Filter(uint16_t new_value,uint8_t Channel_no);
uint16_t ADC_Threshold_Check(uint16_t new_value,uint8_t Channel_no);

//uint16_t Dadc_filter(uint16_t new_sample,uint8_t Channel_no);