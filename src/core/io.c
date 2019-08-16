#include <io.h>

static read_cb_vector_t read_cb_vector;

void iosetpinnout(uint8_t pin, direction_t direction) {
	volatile uint8_t *reg;
	
	switch(pin) {
		case PIN13:
		case PIN12:	
		case PIN11:
		case PIN10:	
		case PIN9:
		case PIN8:
			reg = &DDRB;
			break;
		case PIN7:
		case PIN6:	
		case PIN5:
		case PIN4:	
		case PIN3:
		case PIN2:
		case PIN1:
		case PIN0:
			reg = &DDRD; break;
		default:
			return;
	}
	pin &= 0x0F;

	switch (direction) {
		case output:
			*reg |= _BV(pin);
			break;
		case input:
		default:
			*reg &= ~_BV(pin);
			break;
	}

}

state_t ioreadinput(uint8_t pin) {
	volatile uint8_t *reg;
	switch(pin) {
		case PIN13:
		case PIN12:	
		case PIN11:
		case PIN10:	
		case PIN9:
		case PIN8:
			reg = &PINB;	break;
		case PIN7:
		case PIN6:	
		case PIN5:
		case PIN4:	
		case PIN3:
		case PIN2:
		case PIN1:
		case PIN0:
			reg = &PIND;	break;
		default:
			break;
	}
	pin &= 0x0F;

	if (*reg & _BV(pin))
		return high;
	else
		return low;
}

void iowriteoutput(uint8_t pin, state_t state) {
	volatile uint8_t *reg;
	switch(pin) {
		case PIN13:
		case PIN12:	
		case PIN11:
		case PIN10:	
		case PIN9:
		case PIN8:
			reg = &PORTB;	break;
		case PIN7:
		case PIN6:	
		case PIN5:
		case PIN4:	
		case PIN3:
		case PIN2:
		case PIN1:
		case PIN0:
			reg = &PORTD; 	break;
		default:
			break;
	}
	pin &= 0x0F;

	if (state == high)
		*reg |= _BV(pin);
	else
		*reg &= ~_BV(pin);

}

// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
void ioreadasync(uint8_t pin, read_cb_t read_cb) {
	iosetpinnout(pin, input);
	iowriteoutput(pin, high);

	switch (pin) {
		case PIN0:
			read_cb_vector.read_cb_pcint16 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT16);
    		break;
		case PIN1:
			read_cb_vector.read_cb_pcint17 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT17);
    		break;
		case PIN2:
			read_cb_vector.read_cb_pcint18 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT18);
			break;
		case PIN3:
			read_cb_vector.read_cb_pcint19 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT19);
    		break;
		case PIN4:
			read_cb_vector.read_cb_pcint20 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT20);
    		break;
		case PIN5:
			read_cb_vector.read_cb_pcint21 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT21);
			break;
		case PIN6:
			read_cb_vector.read_cb_pcint22 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT22);
    		break;
		case PIN7:
			read_cb_vector.read_cb_pcint23 = read_cb;
		    PCICR |= (1 << PCIE2);
    		PCMSK2 |= (1 << PCINT23);
    		break;
		case PIN8:
			read_cb_vector.read_cb_pcint0 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT0);
			break;
		case PIN9:
			read_cb_vector.read_cb_pcint1 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT1);
    		break;
		case PIN10:
			read_cb_vector.read_cb_pcint2 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT2);
			break;
		case PIN11:
			read_cb_vector.read_cb_pcint3 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT3);
    		break;
		case PIN12:
			read_cb_vector.read_cb_pcint4 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT4);
			break;
		case PIN13:
			read_cb_vector.read_cb_pcint5 = read_cb;
		    PCICR |= (1 << PCIE0);
    		PCMSK0 |= (1 << PCINT5);
    		break;
	}
	sei();
}

ISR(PCINT0_vect) {
	read_cb_vector.read_cb_pcint0(ioreadinput(PIN8));
}


ISR(PCINT1_vect) {
	read_cb_vector.read_cb_pcint1(ioreadinput(PIN9));
}

ISR(PCINT2_vect) {
	if ((PINB & (1 << PINB2)) == 1)
		read_cb_vector.read_cb_pcint2(low);
	else
		read_cb_vector.read_cb_pcint2(high);
}

ISR(PCINT3_vect) {

	if ((PINB & (1 << PINB3)) == 1)
		read_cb_vector.read_cb_pcint3(low);
	else
		read_cb_vector.read_cb_pcint3(high);
}

ISR(PCINT4_vect) {
	if ((PINB & (1 << PINB4)) == 1)
		read_cb_vector.read_cb_pcint4(low);
	else
		read_cb_vector.read_cb_pcint4(high);
}

ISR(PCINT5_vect) {
	if ((PINB & (1 << PINB5)) == 1)
		read_cb_vector.read_cb_pcint5(low);
	else
		read_cb_vector.read_cb_pcint5(high);
}

ISR(PCINT16_vect) {
	if ((PIND & (1 << PIND0)) == 1)
		read_cb_vector.read_cb_pcint16(low);
	else
		read_cb_vector.read_cb_pcint16(high);
}

ISR(PCINT17_vect) {
	if ((PIND & (1 << PIND1)) == 1)
		read_cb_vector.read_cb_pcint17(low);
	else
		read_cb_vector.read_cb_pcint17(high);
}

ISR(PCINT18_vect) {
	if ((PIND & (1 << PIND2)) == 1)
		read_cb_vector.read_cb_pcint18(low);
	else
		read_cb_vector.read_cb_pcint18(high);
}

ISR(PCINT19_vect) {
	if ((PIND & (1 << PIND3)) == 1)
		read_cb_vector.read_cb_pcint19(low);
	else
		read_cb_vector.read_cb_pcint19(high);
}

ISR(PCINT20_vect) {
	if ((PIND & (1 << PIND4)) == 1)
		read_cb_vector.read_cb_pcint20(low);
	else
		read_cb_vector.read_cb_pcint20(high);
}

ISR(PCINT21_vect) {
	if ((PIND & (1 << PIND5)) == 1)
		read_cb_vector.read_cb_pcint21(low);
	else
		read_cb_vector.read_cb_pcint21(high);
}

ISR(PCINT22_vect) {
	if ((PIND & (1 << PIND6)) == 1)
		read_cb_vector.read_cb_pcint22(low);
	else
		read_cb_vector.read_cb_pcint22(high);
}

ISR(PCINT23_vect) {
	if ((PIND & (1 << PIND7)) == 1)
		read_cb_vector.read_cb_pcint23(low);
	else
		read_cb_vector.read_cb_pcint23(high);
}