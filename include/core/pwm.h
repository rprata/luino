#ifndef __PWM__
#define __PWM__
#include <io.h>

typedef enum { pwm_prescaler_none = 1, 
			   pwm_prescaler_8 = 8, 
			   pwm_prescaler_64 = 64, 
			   pwm_prescaler_256 = 256,
			   pwm_prescaler_1024 = 1024
} pwm_prescaler_t;

typedef struct
{
	uint8_t pin;
	pwm_prescaler_t prescaler;
	float duty;
	uint16_t * reg;
	uint16_t * timerA;
	uint16_t * timerB;
} pwm_st;

pwm_st * pwmsetup(uint8_t pin, pwm_prescaler_t prescaler, float duty);
void pwmduty(pwm_st * pwm, float duty);

#endif