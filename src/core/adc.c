#include <adc.h>

void adcsetup(adc_resolution_t resolution, adc_prescaler_t prescaler)
{
	if (resolution == adc_resolution_8) 
    	ADMUX = (1 << ADLAR) | (1 << REFS0);
    else
    	ADMUX = (1 << REFS0); 

	s_resolution = resolution;

    switch (prescaler) {
    	case adc_prescaler_128:
    		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    		break;
    	case adc_prescaler_64:
    		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
    		break;
    	case adc_prescaler_32:
    		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
    		break;
    	case adc_prescaler_16:
    		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
    		break;
    	case adc_prescaler_8:
    		ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    		break;
    	case adc_prescaler_4:
    		ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
    		break;
    	case adc_prescaler_2:
    		ADCSRA = (1 << ADEN) | (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
    		break;
    }
}

uint16_t adcread(uint8_t channel)
{
    channel &= 0b00000111;
    ADMUX = (ADMUX & 0xF8) | channel;
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));

    if (s_resolution == adc_resolution_8)
    	return (ADCH);
    else
    	return (ADC);
}

uint8_t adcbit2percentage(uint16_t val) 
{
	if (s_resolution == adc_resolution_8)
		return (val * 100) / 255;
	else
		return (val * 100) / 1023;
}

