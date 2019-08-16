#include <alarm.h>
#include <io.h>

static alarm_cb_t s_alarm_cb;
static uint16_t s_time_ms;

void alarmsetup(uint16_t time_ms, alarm_cb_t alarm_cb) 
{
	TCCR1A = 0;
	TCCR1B = 0;
    TCCR1B |= (1 << CS10) | (1 << CS12);
    TCNT1 = 65536 - ((16000000 / 1024) * ((float)time_ms / 1000.0));
    TIMSK1 |= (1 << TOIE1);
    sei();
    s_alarm_cb = alarm_cb;
    s_time_ms = time_ms;
}

void registeralarmcb(alarm_cb_t alarm_cb) 
{
	s_alarm_cb = alarm_cb;
}

void timersetup(uint16_t time_ms) 
{
	s_time_ms = time_ms;
}

void alarmshutdown(void) 
{
	TCCR1A = 0;
	TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;
}

ISR(TIMER1_OVF_vect) 
{
	TCNT1 = 65536 - ((16000000 / 1024) * ((float)s_time_ms / 1000.0));
	s_alarm_cb();
}