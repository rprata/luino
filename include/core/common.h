#ifndef __COMMON_H__
#define __COMMON_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef OWNER
#define EXTERN
#else
#define EXTERN extern	/* declares external variables */
#endif

typedef enum { false = 0 , true } bool;

#define SERIAL_PORT 0	
#define BAUD 115200

#endif