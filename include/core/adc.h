#ifndef __ADC__
#define __ADC__

#include <common.h>

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

typedef enum { adc_resolution_10 = 0 , adc_resolution_8 } adc_resolution_t;

typedef enum { adc_prescaler_2 = 2, 
			   adc_prescaler_4 = 4, 
			   adc_prescaler_8 = 8, 
			   adc_prescaler_16 = 16, 
			   adc_prescaler_32 = 32, 
			   adc_prescaler_64 = 64, 
			   adc_prescaler_128 = 128 
} adc_prescaler_t;

static adc_resolution_t s_resolution = adc_resolution_10;

void adcsetup(adc_resolution_t resolution, adc_prescaler_t prescaler);
uint16_t adcread(uint8_t channel);
uint8_t adcbit2percentage(uint16_t val);

#endif