#include <pwm.h>

pwm_st * pwmsetup(uint8_t pin, pwm_prescaler_t prescaler, float duty) {
	
	if (duty > 1.0) duty = 1.0;
	if (duty < 0.0) duty = 0.0;

	pwm_st * pwm = (pwm_st *) malloc(sizeof(pwm_st));
	pwm->pin = pin;
	pwm->prescaler = prescaler;
	pwm->duty = duty;

	iosetpinnout(pin, output);
	sei();
	switch (pin) {
		case PIN6:
			pwm->reg = &OCR0A;
			pwm->timerA = &TCCR0A;
			pwm->timerB = &TCCR0B;
			*pwm->timerA |= (1 << COM0A1); 
			break;
		case PIN5:
			pwm->reg = &OCR0B;
			pwm->timerA = &TCCR0A;
			pwm->timerB = &TCCR0B;
			*pwm->timerA |= (1 << COM0B1); 
			break;
		case PIN9:
		    ICR1 = 0xFFFF;
			pwm->reg = &OCR1A;
			pwm->timerA = &TCCR1A;
			pwm->timerB = &TCCR1B;
			*pwm->timerA |= (1 << COM1A1) | (1 << COM1B1);
			break;
		case PIN10:
		    ICR1 = 0xFFFF;
			pwm->reg = &OCR1B;
			pwm->timerA = &TCCR1A;
			pwm->timerB = &TCCR1B;
			*pwm->timerA |= (1 << COM1A1) | (1 << COM1B1);
			break;
		case PIN11:
			pwm->reg = &OCR2A;
			pwm->timerA = &TCCR2A;
			pwm->timerB = &TCCR2B;
			*pwm->timerA |= (1 << COM2A1); 
			break;
		case PIN3:
			pwm->reg = &OCR2B;
			pwm->timerA = &TCCR2A;
			pwm->timerB = &TCCR2B;
			*pwm->timerA |= (1 << COM2B1); 
			break;
	}

	if (pwm->timerA == &TCCR0A) {
	    *pwm->timerA |= (1 << WGM01) | (1 << WGM00);
	    switch(prescaler) {
	    	case pwm_prescaler_none:
	    		*pwm->timerB |= (1 << CS00);
	    		break;
	    	case pwm_prescaler_8:
	    		*pwm->timerB |= (1 << CS01);
	    		break;
	    	case pwm_prescaler_64:
	    		*pwm->timerB |= (1 << CS01) | (1 << CS00);
	    		break;
	    	case pwm_prescaler_256:
	    		*pwm->timerB |= (1 << CS02);
	    		break;
	    	case pwm_prescaler_1024:
	    		*pwm->timerB |= (1 << CS01) |  (1 << CS00);
	    		break;
	    }
	    *pwm->reg = 255 * duty;
	}

	if (pwm->timerA == &TCCR1A) {
	    *pwm->timerA |= (1 << WGM11);
	    *pwm->timerB |= (1 << WGM12) | (1 << WGM13);
	    switch(prescaler) {
	    	case pwm_prescaler_none:
	    		*pwm->timerB |= (1 << CS10);
	    		break;
	    	case pwm_prescaler_8:
	    		*pwm->timerB |= (1 << CS11);
	    		break;
	    	case pwm_prescaler_64:
	    		*pwm->timerB |= (1 << CS11) | (1 << CS10);
	    		break;
	    	case pwm_prescaler_256:
	    		*pwm->timerB |= (1 << CS12);
	    		break;
	    	case pwm_prescaler_1024:
	    		*pwm->timerB |= (1 << CS11) |  (1 << CS10);
	    		break;
	    }
	    *pwm->reg = ((uint8_t)(255 * duty) << 8) | 0x00FF;
	}

	if (pwm->timerA == &TCCR2A) {
	    *pwm->timerA |= (1 << WGM21) | (1 << WGM20);
	    switch(prescaler) {
	    	case pwm_prescaler_none:
	    		*pwm->timerB |= (1 << CS20);
	    		break;
	    	case pwm_prescaler_8:
	    		*pwm->timerB |= (1 << CS21);
	    		break;
	    	case pwm_prescaler_64:
	    		*pwm->timerB |= (1 << CS21) | (1 << CS20);
	    		break;
	    	case pwm_prescaler_256:
	    		*pwm->timerB |= (1 << CS22);
	    		break;
	    	case pwm_prescaler_1024:
	    		*pwm->timerB |= (1 << CS21) |  (1 << CS20);
	    		break;
	    }
	    *pwm->reg = 255 * duty;
	}
	return pwm;
}


void pwmduty(pwm_st * pwm, float duty) {
	pwm->duty = duty;
	if (pwm->timerA == &TCCR1A)
		*pwm->reg = (uint16_t)((uint8_t)(255 * duty) << 8) | 0x00FF;
	else
		*pwm->reg = 255 * duty;
}