#ifndef __IO_H__
#define __IO_H__

#include <common.h>

typedef enum { input = 0 , output } direction_t;
typedef enum { low = 0 , high } state_t;

typedef void (*read_cb_t)(state_t state);
typedef struct {
	read_cb_t read_cb_pcint0;
	read_cb_t read_cb_pcint1;	
	read_cb_t read_cb_pcint2;
	read_cb_t read_cb_pcint3;	
	read_cb_t read_cb_pcint4;
	read_cb_t read_cb_pcint5;
	read_cb_t read_cb_pcint16;
	read_cb_t read_cb_pcint17;		
	read_cb_t read_cb_pcint18;
	read_cb_t read_cb_pcint19;		
	read_cb_t read_cb_pcint20;		
	read_cb_t read_cb_pcint21;		
	read_cb_t read_cb_pcint22;
	read_cb_t read_cb_pcint23;		
} read_cb_vector_t;

#define PIN13 (0xB0  | PORTB5)
#define PIN12 (0xB0  | PORTB4)
#define PIN11 (0xB0  | PORTB3)
#define PIN10 (0xB0  | PORTB2)
#define PIN9  (0xB0  | PORTB1)
#define PIN8  (0xB0  | PORTB0)
#define PIN7  (0xD0  | PORTD7)
#define PIN6  (0xD0  | PORTD6)
#define PIN5  (0xD0  | PORTD5)
#define PIN4  (0xD0  | PORTD4)
#define PIN3  (0xD0  | PORTD3)
#define PIN2  (0xD0  | PORTD2)
#define PIN1  (0xD0  | PORTD1)
#define PIN0  (0xD0  | PORTD0)

void iosetpinnout(uint8_t pin, direction_t direction);
state_t ioreadinput(uint8_t pin); 
void iowriteoutput(uint8_t pin, state_t state);
void ioreadasync(uint8_t pin, read_cb_t read_cb);

#endif